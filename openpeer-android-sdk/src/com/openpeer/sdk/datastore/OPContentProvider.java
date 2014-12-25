/*******************************************************************************
 *
 *  Copyright (c) 2014 , Hookflash Inc.
 *  All rights reserved.
 *  
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  
 *  1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  
 *  The views and conclusions contained in the software and documentation are those
 *  of the authors and should not be interpreted as representing official policies,
 *  either expressed or implied, of the FreeBSD Project.
 *******************************************************************************/
package com.openpeer.sdk.datastore;

import java.util.Arrays;
import java.util.List;

import android.content.ContentProvider;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.UriMatcher;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.ProviderInfo;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.net.Uri;
import android.provider.BaseColumns;
import android.text.TextUtils;
import android.util.Log;

import com.openpeer.sdk.datastore.DatabaseContracts.AccountEntry;
import com.openpeer.sdk.datastore.DatabaseContracts.AssociatedIdentityEntry;
import com.openpeer.sdk.datastore.DatabaseContracts.AvatarEntry;
import com.openpeer.sdk.datastore.DatabaseContracts.CallEntry;
import com.openpeer.sdk.datastore.DatabaseContracts.ConversationEntry;
import com.openpeer.sdk.datastore.DatabaseContracts.IdentityContactEntry;
import com.openpeer.sdk.datastore.DatabaseContracts.MessageEntry;
import com.openpeer.sdk.datastore.DatabaseContracts.OpenpeerContactEntry;
import com.openpeer.sdk.datastore.DatabaseContracts.ParticipantEntry;
import com.openpeer.sdk.datastore.DatabaseContracts.RolodexContactEntry;
import com.openpeer.sdk.datastore.DatabaseContracts.WindowViewEntry;
import com.openpeer.sdk.model.GroupChatMode;

public class OPContentProvider extends ContentProvider {

    private OPDatabaseHelper mOpenHelper;

    private static final String TAG = OPContentProvider.class.getSimpleName();

    UriMatcher mUriMatcher;
    static String sAuthority = "";
    static final String SCHEME = "content://";
    private static OPContentProvider instance;

    static enum MatcherInfo {
        ACCOUNTS(AccountEntry.TABLE_NAME),
        ACCOUNT(AccountEntry.TABLE_NAME + "/#"),
        IDENTTIIES(AssociatedIdentityEntry.TABLE_NAME),
        IDENTITY(AssociatedIdentityEntry.TABLE_NAME + "/#"),

        CONVERSATION_HISTORY(MessageEntry.URI_PATH_INFO_CONTEXT),//Messages/Events of a conversation
        CONVERSATION_MESSAGE(MessageEntry.URI_PATH_INFO_CONTEXT_ID),//Messages/Events of a conversation

        MESSAGES(MessageEntry.TABLE_NAME),
        MESSAGE(MessageEntry.TABLE_NAME + "/#"),

        CONVERSATIONS(ConversationEntry.URI_PATH_INFO),//Conversations table for direct access
        CONVERSATION(ConversationEntry.URI_PATH_INFO_ID),// Conversation item

        WINDOW_PARTICIPANTS(ParticipantEntry.URI_PATH_INFO),
        WINDOW_PARTICIPANT(ParticipantEntry.URI_PATH_INFO_ID),

        CONVERSATIONS_VIEW(WindowViewEntry.URI_PATH_INFO_CONTEXT),//Conversation informations to construct the conversations view
        OPENPEER_CONTACT(OpenpeerContactEntry.URI_PATH_INFO_ID),
        OPENPEER_CONTACT_DETAIL(OpenpeerContactEntry.URI_PATH_INFO_DETAIL),
        OPENPEER_CONTACT_DETAIL_ID(OpenpeerContactEntry.URI_PATH_INFO_DETAIL_ID),

        // USER(UserEntry.TABLE_NAME + "/#"),

        IDENTITY_CONTACTS(IdentityContactEntry.TABLE_NAME),
        IDENTITY_CONTACT(IdentityContactEntry.TABLE_NAME + "/#"),

        ROLODEX_CONTACTS(RolodexContactEntry.TABLE_NAME),
        ROLODEX_CONTACT(IdentityContactEntry.TABLE_NAME + "/#"),

        AVATARS(AvatarEntry.TABLE_NAME),
        AVATAR(AvatarEntry.TABLE_NAME + "/#"),

        CALLS(CallEntry.TABLE_NAME),
        CALL(CallEntry.TABLE_NAME + "/#");

        private final String mPath;

        private MatcherInfo(final String path) {
            mPath = path;
        }

        public String getPath() {
            return mPath;
        }
    }

    /**
     * Helper class to get the content URI with the authority provided by application and path string defined in DatabaseContracts
     * 
     * @param path
     * @return
     */
    public static Uri getContentUri(String path) {
        return Uri.parse(SCHEME + sAuthority + path);
    }

    @Override
    public int delete(Uri uri, String selection, String[] selectionArgs) {
        MatcherInfo value = MatcherInfo.values()[mUriMatcher.match(uri)];
        StringBuilder stringBuilder = new StringBuilder();

        String table = uri.getLastPathSegment();
        SQLiteDatabase db = mOpenHelper.getWritableDatabase();
        int result = db.delete(table, selection, selectionArgs);
        return result;
    }

    @Override
    public String getType(Uri uri) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public int bulkInsert(Uri uri, ContentValues[] values) {
        int result = super.bulkInsert(uri, values);
        // test();
        return result;
    }

    @Override
    public Uri insert(Uri uri, ContentValues values) {
        Uri result = null;
        MatcherInfo value = MatcherInfo.values()[mUriMatcher.match(uri)];
        StringBuilder stringBuilder = new StringBuilder();
        switch (value) {
        case MESSAGES:
            return insertMessage(uri, values);

        default:
            String tableName = null;
            List<String> pathSegments = uri.getPathSegments();
            switch (pathSegments.size()) {
            case 1:
                tableName = pathSegments.get(0);
                break;
            case 2:
                tableName = pathSegments.get(1);
                break;
            }
            if (tableName != null) {
                SQLiteDatabase db = mOpenHelper.getWritableDatabase();
                long rowId = db.insert(tableName, null, values);

                if (rowId != -1) {
                    getContext().getContentResolver().notifyChange(uri, null);
                    return ContentUris.withAppendedId(uri, rowId);
                }
            }
        }
        return result;
    }

    private Uri insertMessage(Uri uri, ContentValues values) {
        SQLiteDatabase db = mOpenHelper.getWritableDatabase();
        long result = db.insert(DatabaseContracts.MessageEntry.TABLE_NAME,
                null, values);
        getContext().getContentResolver().notifyChange(
                ContentUris.withAppendedId(uri, result), null);
        notifyChatHistoryChange();
        return uri;
    }

    @Override
    public boolean onCreate() {
        mOpenHelper = OPDatabaseHelper.getInstance(getContext());
        initMatcher();
        instance = this;
        return true;
    }

    @Override
    public Cursor query(Uri uri, String[] projection, String selection,
            String[] selectionArgs, String sortOrder) {
        int index = mUriMatcher.match(uri);
        if (index == -1) {
            Log.d(TAG, "Illegal URI " + uri);
        }
        MatcherInfo value = MatcherInfo.values()[index];
        StringBuilder stringBuilder = new StringBuilder();

        switch (value) {
        case CONVERSATION_HISTORY:{
            stringBuilder.append(DatabaseContracts.QUERY_MESSAGES)
                .append("where " + MessageEntry.COLUMN_CONTEXT_ID)
                .append("=?");

            stringBuilder.append(" union " + DatabaseContracts.QUERY_CALL)
                .append(" where ")
                .append(MessageEntry.COLUMN_CONTEXT_ID + "=?");
            stringBuilder
                .append(" union "
                            + DatabaseContracts.QUERY_CONVERSATION_EVENT)
                .append(" and ").append("c.conversation_id=?");
            stringBuilder.append(" order by time");

            String conversationId = uri.getLastPathSegment();
            String args[];

            args = new String[]{conversationId, conversationId, conversationId};

            Cursor contextCursor = mOpenHelper.getReadableDatabase().rawQuery(
                stringBuilder.toString(), args);
            contextCursor.setNotificationUri(getContext().getContentResolver(), uri);
            return contextCursor;
        }
        case OPENPEER_CONTACT:
            return queryOpenPeerContact(uri, projection, selection,
                    selectionArgs,
                    sortOrder);
        case OPENPEER_CONTACT_DETAIL:
            return queryOpenPeerContactDetail(uri, projection,
                    selection,
                    selectionArgs,
                    sortOrder);
        case OPENPEER_CONTACT_DETAIL_ID:
            if (TextUtils.isEmpty(selection)) {
                selection = "oc._id=" + uri.getLastPathSegment();
            } else {
                selection = "oc._id=" + uri.getLastPathSegment() + " and "
                        + selection;
            }
            return queryOpenPeerContactDetail(uri, projection, selection,
                    selectionArgs,
                    sortOrder);
        case ROLODEX_CONTACTS:
            return queryRolodexContacts(uri, projection, selection,
                    selectionArgs,
                    sortOrder);
        case CONVERSATIONS_VIEW:
            return queryConversationHisotry(uri, projection, selection,
                                            selectionArgs,
                                            sortOrder);

        default:
            break;

        }

        String tableName = null;
        List<String> pathSegments = uri.getPathSegments();
        switch (pathSegments.size()) {
        case 1:
            tableName = pathSegments.get(0);
            break;
        case 2:
            tableName = pathSegments.get(1);
            break;
        }
        if (tableName != null) {
            SQLiteDatabase db = mOpenHelper.getReadableDatabase();

            Cursor cursor = db.query(
                    tableName,
                    projection, // The columns to return from the query
                    selection, // The columns for the where clause
                    selectionArgs, // The values for the where clause
                    null, // don't group the rows
                    null, // don't filter by row groups
                    sortOrder // The sort order
                    );
            cursor.setNotificationUri(getContext().getContentResolver(), uri);
            return cursor;
        } else {
            return null;
        }
    }

    /**
     * @return
     */
    private Cursor queryConversationHisotry(Uri uri, String[] projection,
                                            String selection, String[] selectionArgs, String sortOrder) {
        String rawQuery = DatabaseContracts.QUERY_OPENPEER_CHATS_CONTEXT_BASED;
        if (!TextUtils.isEmpty(selection)) {
            rawQuery = rawQuery + " where " + selection;
        }
        Cursor cursor = mOpenHelper.getReadableDatabase().rawQuery(rawQuery,
                selectionArgs);
        cursor.setNotificationUri(getContext().getContentResolver(), uri);

        return cursor;
    }

    /**
     * Should be used to retrieve user details like identityContact info
     * 
     * @param uri
     * @param projection
     * @param selection
     * @param selectionArgs
     * @param sortOrder
     * @return
     */
    private Cursor queryOpenPeerContact(Uri uri, String[] projection,
            String selection, String[] selectionArgs, String sortOrder) {
        long id = Integer.parseInt(uri.getLastPathSegment());
        // String rawQuery =
        // "select oc._id as openpeer_id, oc.stable_id, pf.peer_uri as peer_uri,pf.peerfile_public as peerfile_public,oc.name as name, oc.identity_uri as identity_uri,ip.identity_provider_domain as identity_provider_domain from openpeer_contact oc"
        // +
        // " left join peefile_public pf on oc.peerfile_id=left join rolodex_contact rc on oc._id=rc.openpeer_contact_id left join identity_contact ic on rc.identity_contact_id=ic._id left join identity_provider ip on rc.identity_provider_id=ip._id"
        // + " where oc.id=" + id;
        String rawQuery = "select oc._id as openpeer_contact_id, oc.stable_id as stable_id, pf.peer_uri as peer_uri,pf.peerfile_public as peerfile_public from openpeer_contact oc left join peerfile_public pf on oc.peerfile_id=pf._id where oc._id="
                + id;
        Cursor cursor = mOpenHelper.getReadableDatabase().rawQuery(rawQuery,
                null);
        return cursor;
    }

    /**
     * Should be used to retrieve user details like identityContact info
     * 
     * @param uri
     * @param projection
     * @param selection
     * @param selectionArgs
     * @param sortOrder
     * @return
     */
    private Cursor queryOpenPeerContactDetail(Uri uri, String[] projection,
            String selection, String[] selectionArgs, String sortOrder) {
        String rawQuery = null;

        if (!TextUtils.isEmpty(selection)) {
            rawQuery = DatabaseContracts.QUERY_OPENPEER_CONTACT_DETAIL
                    + " where "
                    + selection;
        } else {
            rawQuery = DatabaseContracts.QUERY_OPENPEER_CONTACT_DETAIL;
        }
        Cursor cursor = mOpenHelper.getReadableDatabase().rawQuery(
                rawQuery,
                selectionArgs);
        return cursor;
    }

    @Override
    public int update(Uri uri, ContentValues values, String selection,
            String[] selectionArgs) {
        int result;
        MatcherInfo value = MatcherInfo.values()[mUriMatcher.match(uri)];

        switch (value) {
        case CONVERSATION_HISTORY:
            return updateMessages(uri, values, selection,
                    selectionArgs);
        case CONVERSATIONS:

        default:
            String table = uri.getLastPathSegment();
            SQLiteDatabase db = mOpenHelper.getWritableDatabase();
            result = db.update(table, values, selection, selectionArgs);
        }
        return result;
    }

    private int updateMessages(Uri uri, ContentValues values, String selection,
            String[] selectionArgs) {
        SQLiteDatabase db = mOpenHelper.getReadableDatabase();
        String conversationId = uri.getLastPathSegment();
        String conversationCondition = ConversationEntry.COLUMN_CONVERSATION_ID + "=?";
        if (!TextUtils.isEmpty(selection)) {
            selection = selection + " and " + conversationCondition;
        } else {
            selection = conversationCondition;
        }
        if (selectionArgs != null) {
            selectionArgs = Arrays.copyOf(selectionArgs, selectionArgs.length + 1);
            selectionArgs[selectionArgs.length - 1] = conversationId;
        } else {
            selectionArgs = new String[]{conversationId};
        }
        int result = db.update(MessageEntry.TABLE_NAME, values, selection, selectionArgs);
        if (result != 0) {
            getContext().getContentResolver().notifyChange(uri, null);
            notifyChatHistoryChange();
        }
        return result;
    }

    private int updateContacts(Uri uri, ContentValues values, String selection,
            String[] selectionArgs) {
        // TODO Auto-generated method stub
        return 0;
    }

    void initMatcher() {
        try {
            ProviderInfo providers[] = this
                    .getContext()
                    .getPackageManager()
                    .getPackageInfo(getContext().getPackageName(),
                            PackageManager.GET_PROVIDERS).providers;
            if (providers != null && providers.length > 0) {
                String myName = OPContentProvider.class.getCanonicalName();
                for (ProviderInfo provider : providers) {
                    if (myName.equals(provider.name)) {
                        sAuthority = provider.authority;
                    }
                }
            }
        } catch (NameNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        mUriMatcher = new UriMatcher(UriMatcher.NO_MATCH);

        for (MatcherInfo mi : MatcherInfo.values()) {
            if (!TextUtils.isEmpty(mi.getPath())) {
                mUriMatcher.addURI(sAuthority, mi.getPath(), mi.ordinal());
            }
        }
    }

    Cursor queryRolodexContacts(Uri uri, String[] projection, String selection,
            String[] selectionArgs, String sortOrder) {
        SQLiteDatabase db = mOpenHelper.getReadableDatabase();

        if (projection == null) {
            projection = new String[] {
                    RolodexContactEntry._ID,
                    RolodexContactEntry.COLUMN_OPENPEER_CONTACT_ID,
                    RolodexContactEntry.COLUMN_CONTACT_NAME,
                    RolodexContactEntry.COLUMN_IDENTITY_URI };
        }
        String where = "associated_identity_id in (select _id from associated_identity where account_id=(select _id from account where logged_in=1))";
        if (selection != null) {
            where = where + " and " + selection;
        }
        Cursor cursor = db.query(
                DatabaseContracts.RolodexContactEntry.TABLE_NAME,
                projection, // The columns to return from the query
                where, // The columns for the where clause
                selectionArgs, // The values for the where clause
                null, // don't group the rows
                null, // don't filter by row groups
                sortOrder // The sort order
                );
        cursor.setNotificationUri(getContext().getContentResolver(), uri);
        return cursor;
    }

    Cursor queryMessages(Uri uri, String[] projection, String selection,
            String[] selectionArgs, String sortOrder) {
        SQLiteDatabase db = mOpenHelper.getReadableDatabase();

        Cursor cursor = db.query(
                DatabaseContracts.MessageEntry.TABLE_NAME,
                projection, // The columns to return from the query
                selection, // The columns for the where clause
                selectionArgs, // The values for the where clause
                null, // don't group the rows
                null, // don't filter by row groups
                sortOrder // The sort order
                );
        Log.d("test",
                "query uri for messages " + uri + " result "
                        + cursor.getCount());
        cursor.setNotificationUri(getContext().getContentResolver(), uri);
        return cursor;
    }

    private Cursor queryMessage(Uri uri, String[] projection, String selection,
            String[] selectionArgs, String sortOrder) {
        SQLiteDatabase db = mOpenHelper.getReadableDatabase();

        String finalWhere = MessageEntry._ID + "=" + uri.getLastPathSegment();
        if (selection != null) {
            finalWhere = finalWhere + " AND " + selection;
        }
        Cursor cursor = db.query(
                DatabaseContracts.MessageEntry.TABLE_NAME,
                projection, // The columns to return from the query
                finalWhere, // The columns for the where clause
                selectionArgs, // The values for the where clause
                null, // don't group the rows
                null, // don't filter by row groups
                sortOrder // The sort order
                );
        Log.d("test",
                "query uri for message " + uri + " result " + cursor.getCount());
        cursor.setNotificationUri(getContext().getContentResolver(), uri);
        return cursor;
    }

    Cursor queryWindows(Uri uri, String[] projection, String selection,
            String[] selectionArgs, String sortOrder) {
        SQLiteDatabase db = mOpenHelper.getReadableDatabase();
        Cursor cursor = db.query(
                DatabaseContracts.WindowViewEntry.TABLE_NAME,
                projection, // The columns to return from the query
                selection, // The columns for the where clause
                selectionArgs, // The values for the where clause
                null, // don't group the rows
                null, // don't filter by row groups
                sortOrder // The sort order
                );
        Log.d("test", "query uri " + uri + " result " + cursor.getCount());
        cursor.setNotificationUri(getContext().getContentResolver(), uri);
        return cursor;
    }

    void notifyChatHistoryChange() {
            getContext().getContentResolver().notifyChange(
                    getContentUri(WindowViewEntry.URI_PATH_INFO_CONTEXT), null);
    }

    /**
     * @return
     */
    public static OPContentProvider getInstance() {
        // TODO Auto-generated method stub
        return instance;
    }

    static String getIdClause(Uri uri) {
        return BaseColumns._ID + "=" + uri.getLastPathSegment();
    }

    static String combinedIdClause(String selection, Uri uri) {
        if (TextUtils.isEmpty(selection)) {
            selection = getIdClause(uri);
        } else {
            selection = selection + getIdClause(uri);
        }
        return selection;
    }
}
