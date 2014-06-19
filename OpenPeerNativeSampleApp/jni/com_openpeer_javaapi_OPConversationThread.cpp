#include "com_openpeer_javaapi_OPStackMessageQueue.h"
#include "openpeer/core/IConversationThread.h"
#include "openpeer/core/IContact.h"
#include "openpeer/core/IHelper.h"
#include "openpeer/core/ILogger.h"
#include "OpenPeerCoreManager.h"

#include <android/log.h>

#include "globals.h"

using namespace openpeer::core;

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    toString
 * Signature: (Lcom/openpeer/javaapi/MessageDeliveryStates;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_openpeer_javaapi_OPConversationThread_toString__Lcom_openpeer_javaapi_MessageDeliveryStates_2
(JNIEnv *, jclass, jobject)
{

}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    toString
 * Signature: (Lcom/openpeer/javaapi/ContactStates;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_openpeer_javaapi_OPConversationThread_toString__Lcom_openpeer_javaapi_ContactStates_2
(JNIEnv *, jclass, jobject)
{

}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    toDebugString
 * Signature: (Lcom/openpeer/javaapi/OPConversationThread;Z)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_openpeer_javaapi_OPConversationThread_toDebugString
(JNIEnv *, jclass, jobject, jboolean)
{

}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    create
 * Signature: (Lcom/openpeer/javaapi/OPAccount;Ljava/util/List;)Lcom/openpeer/javaapi/OPConversationThread;
 */
JNIEXPORT jobject JNICALL Java_com_openpeer_javaapi_OPConversationThread_create
(JNIEnv *, jclass, jobject, jobject identityContacts)
{
	jclass cls;
	jmethodID method;
	jobject object;
	JNIEnv *jni_env = 0;
	IConversationThreadPtr conversationThreadPtr;

	//Core contact profile list
	IdentityContactList coreIdentityContacts;
	IdentityContact coreIdentityContact;

	__android_log_print(ANDROID_LOG_INFO, "com.openpeer.jni", "Conversation thread create called...");

	//check if account is existing
	if(!OpenPeerCoreManager::accountPtr)
	{
		return object;
	}

	//fetch JNI env
	jni_env = getEnv();
	if(jni_env)
	{
		//create return object - java/util/List is interface, ArrayList is implementation
		jclass arrayListClass = findClass("java/util/ArrayList");
		if(jni_env->IsInstanceOf(identityContacts, arrayListClass) != JNI_TRUE)
		{
			return object;
		}

		// Fetch "java.util.List.get(int location)" MethodID
		jmethodID listGetMethodID = jni_env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");
		// Fetch "int java.util.List.size()" MethodID
		jmethodID sizeMethodID = jni_env->GetMethodID( arrayListClass, "size", "()I" );

		// Call "int java.util.List.size()" method and get count of items in the list.
		int listItemsCount = (int)jni_env->CallIntMethod( identityContacts, sizeMethodID );

		//Fetch OPIdentityContact class
		jclass identityContactClass = findClass("com/openpeer/javaapi/OPIdentityContact");

		//FETCH METHODS TO GET INFO FROM JAVA
		//Fetch getStableID method from OPIdentityContact class
		jmethodID getStableIDMethodID = jni_env->GetMethodID( identityContactClass, "getStableID", "()Ljava/lang/String;" );
		//Fetch getPeerFilePublic method from OPIdentityContact class
		jmethodID getPeerFilePublicMethodID = jni_env->GetMethodID( identityContactClass, "getPeerFilePublic", "()Lcom/openpeer/javaapi/OPPeerFilePublic;" );
		//Fetch getIdentityProofBundle method from OPIdentityContact class
		jmethodID getIdentityProofBundleMethodID = jni_env->GetMethodID( identityContactClass, "getIdentityProofBundle", "()Ljava/lang/String;" );
		//Fetch getPriority method from OPIdentityContact class
		jmethodID getPriorityMethodID = jni_env->GetMethodID( identityContactClass, "getPriority", "()I" );
		//Fetch getWeight method from OPIdentityContact class
		jmethodID getWeightMethodID = jni_env->GetMethodID( identityContactClass, "getWeight", "()I" );
		//Fetch getLastUpdated method from OPIdentityContact class
		jmethodID getLastUpdatedMethodID = jni_env->GetMethodID( identityContactClass, "getLastUpdated", "()Landroid/text/format/Time;" );
		//Fetch getExpires method from OPIdentityContact class
		jmethodID getExpiresMethodID = jni_env->GetMethodID( identityContactClass, "getExpires", "()Landroid/text/format/Time;" );

		//get rolodex contact setter methods
		///////////////////////////////////////////////////////////////
		// GET ROLODEX CONTACT FIELDS
		//////////////////////////////////////////////////////////////

		//Fetch setDisposition method from OPDownloadedRolodexContacts class
		//jclass dispositionClass = findClass("com/openpeer/javaapi/OPRolodexContact$Dispositions");
		//jmethodID dispositionConstructorMethodID = jni_env->GetMethodID(cls, "<init>", "()V");
		jmethodID getDispositionMethodID = jni_env->GetMethodID( identityContactClass, "getDisposition", "()Lcom/openpeer/javaapi/OPRolodexContact$Dispositions;" );
		//Fetch setIdentityURI method from OPDownloadedRolodexContacts class
		jmethodID getIdentityURIMethodID = jni_env->GetMethodID( identityContactClass, "getIdentityURI", "()Ljava/lang/String;" );
		//Fetch setIdentityProvider method from OPDownloadedRolodexContacts class
		jmethodID getIdentityProviderMethodID = jni_env->GetMethodID( identityContactClass, "getIdentityProvider", "()Ljava/lang/String;" );
		//Fetch setName method from OPDownloadedRolodexContacts class
		jmethodID getNameMethodID = jni_env->GetMethodID( identityContactClass, "getName", "()Ljava/lang/String;" );
		//Fetch setProfileURL method from OPDownloadedRolodexContacts class
		jmethodID getProfileURLMethodID = jni_env->GetMethodID( identityContactClass, "getProfileURL", "()Ljava/lang/String;" );
		//Fetch setVProfileURL method from OPDownloadedRolodexContacts class
		jmethodID getVProfileURLMethodID = jni_env->GetMethodID( identityContactClass, "getVProfileURL", "()Ljava/lang/String;" );
		//Fetch setAvatars method from OPDownloadedRolodexContacts class
		jmethodID getAvatarsMethodID = jni_env->GetMethodID( identityContactClass, "getAvatars", "()Ljava/util/List;");

		for( int i=0; i<listItemsCount; ++i )
		{
			// Call "java.util.List.get" method and get IdentityContact object by index.
			jobject identityContactObject = jni_env->CallObjectMethod( identityContacts, listGetMethodID, i );

			if( identityContactObject != NULL )
			{
				//CALL METHODS TO FETCH INFO FROM JAVA
				// Call getStableID method to fetch stable ID from OPIdentityContact
				jstring stableID = (jstring)jni_env->CallObjectMethod( identityContactObject, getStableIDMethodID );
				const char *nativeString = jni_env->GetStringUTFChars(stableID, 0);
				jni_env->ReleaseStringUTFChars(stableID, nativeString);

				// Call getPeerFilePublic method to fetch peer file public from OPIdentityContact
				jobject peerFilePublic = jni_env->CallObjectMethod( identityContactObject, getPeerFilePublicMethodID );

				// Call getIdentityProofBundle method to fetch identity proof bundle from OPIdentityContact
				jstring identityProofBundle = (jstring)jni_env->CallObjectMethod( identityContactObject, getIdentityProofBundleMethodID );

				// Call getPriority method to fetch priority from OPIdentityContact
				jint priority = jni_env->CallIntMethod( identityContactObject, getPriorityMethodID );

				// Call getWeight method to fetch priority from OPIdentityContact
				jint weight = jni_env->CallIntMethod( identityContactObject, getWeightMethodID );

				// Call getLastUpdated method to fetch last updated from OPIdentityContact
				jobject lastUpdated = jni_env->CallObjectMethod( identityContactObject, getLastUpdatedMethodID );

				// Call getExpires method to fetch expires from OPIdentityContact
				jobject expires = jni_env->CallObjectMethod( identityContactObject, getExpiresMethodID );

				//GET ROLODEX CONTACTS FIELDS
				jobject disposition = jni_env->CallObjectMethod( identityContactObject, getDispositionMethodID );
				// Call getStableID method to fetch stable ID from OPIdentityContact
				jstring identityURI = (jstring)jni_env->CallObjectMethod( identityContactObject, getIdentityURIMethodID );

				jstring identityProvider = (jstring)jni_env->CallObjectMethod( identityContactObject, getIdentityProviderMethodID );

				jstring profileURL = (jstring)jni_env->CallObjectMethod( identityContactObject, getProfileURLMethodID );

				jstring vProfileURL = (jstring)jni_env->CallObjectMethod( identityContactObject, getVProfileURLMethodID );

				jstring name = (jstring)jni_env->CallObjectMethod( identityContactObject, getNameMethodID );


				coreIdentityContact.mDisposition = (RolodexContact::Dispositions) OpenPeerCoreManager::getIntValueFromEnumObject(disposition, "com/openpeer/javaapi/OPRolodexContact$Dispositions");
				coreIdentityContact.mIdentityProvider = jni_env->GetStringUTFChars(identityProvider, NULL);
				coreIdentityContact.mIdentityURI = jni_env->GetStringUTFChars(identityURI, NULL);
				coreIdentityContact.mProfileURL = jni_env->GetStringUTFChars(profileURL, NULL);
				coreIdentityContact.mVProfileURL = jni_env->GetStringUTFChars(vProfileURL, NULL);
				coreIdentityContact.mName = jni_env->GetStringUTFChars(name, NULL);

				jclass cls = findClass("com/openpeer/javaapi/OPPeerFilePublic");
				jfieldID fid = jni_env->GetFieldID(cls, "mPeerFileString", "Ljava/lang/String;");
				jstring peerFileString = (jstring) jni_env->GetObjectField(peerFilePublic, fid);
				String corePeerFileString = jni_env->GetStringUTFChars(peerFileString, NULL);
				ElementPtr peerFilePublicEl = IHelper::createElement(corePeerFileString);
				coreIdentityContact.mPeerFilePublic = IHelper::createPeerFilePublic(peerFilePublicEl);


				//FILL IN CORE IDENTITY CONTACT STRUCTURE WITH DATA FROM JAVA

				//Add stableID to IdentityContact structure
				coreIdentityContact.mStableID = jni_env->GetStringUTFChars(stableID, NULL);

				//Add peerFilePublic to IdentityContact structure
				//TODO will not implement now

				//Add identityProofBundle to IdentityContact structure
				String identityProofBundleString = jni_env->GetStringUTFChars(identityProofBundle, NULL);
				coreIdentityContact.mIdentityProofBundleEl = IHelper::createElement(identityProofBundleString);

				//Add priority to IdentityContact structure
				coreIdentityContact.mPriority = priority;

				//Add weight to IdentityContact structure
				coreIdentityContact.mWeight = weight;

				//Add last updated to IdentityContact structure
				jclass timeCls = findClass("android/text/format/Time");
				jmethodID timeMethodID   = jni_env->GetMethodID(timeCls, "toMillis", "(Z)J");
				jlong longValue = jni_env->CallLongMethod(lastUpdated, timeMethodID, false);
				Time t = boost::posix_time::from_time_t(longValue/1000) + boost::posix_time::millisec(longValue % 1000);
				coreIdentityContact.mLastUpdated = t;

				//Add expires to IdentityContact structure
				//jclass timeCls = findClass("android/text/format/Time");
				//jmethodID timeMethodID   = jni_env->GetMethodID(timeCls, "toMillis", "(Z)J");
				longValue = jni_env->CallLongMethod(expires, timeMethodID, false);
				t = boost::posix_time::from_time_t(longValue/1000) + boost::posix_time::millisec(longValue % 1000);
				coreIdentityContact.mExpires = t;

//				__android_log_print(ANDROID_LOG_INFO, "com.openpeer.jni", "mIdentityProvider %s ", coreIdentityContact.mIdentityProvider.c_str());
//				__android_log_print(ANDROID_LOG_INFO, "com.openpeer.jni", "mIdentityProofBundleEl %s ", IHelper::convertToString(coreIdentityContact.mIdentityProofBundleEl).c_str());
//				__android_log_print(ANDROID_LOG_INFO, "com.openpeer.jni", "mIdentityURI %s ", coreIdentityContact.mIdentityURI.c_str());
//				__android_log_print(ANDROID_LOG_INFO, "com.openpeer.jni", "mProfileURL %s ", coreIdentityContact.mProfileURL.c_str());
//				__android_log_print(ANDROID_LOG_INFO, "com.openpeer.jni", "mName %s ", coreIdentityContact.mName.c_str());
//				__android_log_print(ANDROID_LOG_INFO, "com.openpeer.jni", "disposition %d ", coreIdentityContact.mDisposition);

				//add core identity contacts to list
				coreIdentityContacts.push_front(coreIdentityContact);

			}
		}
	}

	conversationThreadPtr = IConversationThread::create(OpenPeerCoreManager::accountPtr, coreIdentityContacts);

	if(conversationThreadPtr)
	{
		jni_env = getEnv();
		if(jni_env)
		{
			cls = findClass("com/openpeer/javaapi/OPConversationThread");
			method = jni_env->GetMethodID(cls, "<init>", "()V");
			object = jni_env->NewObject(cls, method);

			jfieldID fid = jni_env->GetFieldID(cls, "nativeClassPointer", "J");
			jlong convThread = (jlong) conversationThreadPtr.get();
			jni_env->SetLongField(object, fid, convThread);

			__android_log_print(ANDROID_LOG_INFO, "com.openpeer.jni",
					"CorePtr raw = %p, ptr as long = %Lu",conversationThreadPtr.get(), convThread);

			OpenPeerCoreManager::coreConversationThreadList.push_back(conversationThreadPtr);
			conversationThreadMap.insert(std::pair<jobject, IConversationThreadPtr>(object, conversationThreadPtr));

		}
	}
	return object;
}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    getConversationThreads
 * Signature: (Lcom/openpeer/javaapi/OPAccount;)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_com_openpeer_javaapi_OPConversationThread_getConversationThreads
(JNIEnv *, jclass, jobject)
{
	jclass cls;
	jmethodID method;
	jobject returnListObject;
	JNIEnv *jni_env = 0;

	//Core identity list
	ConversationThreadListPtr coreConversationThreads;


	//take associated identities from core
	if(OpenPeerCoreManager::accountPtr)
	{
		coreConversationThreads = IConversationThread::getConversationThreads(OpenPeerCoreManager::accountPtr);
	}

	//fetch JNI env
	jni_env = getEnv();
	if(jni_env)
	{
		//create return object - java/util/List is interface, ArrayList is implementation
		jclass returnListClass = findClass("java/util/ArrayList");
		jmethodID listConstructorMethodID = jni_env->GetMethodID(returnListClass, "<init>", "()V");
		returnListObject = jni_env->NewObject(returnListClass, listConstructorMethodID);


		//fetch List.add object
		jmethodID listAddMethodID = jni_env->GetMethodID(returnListClass, "add", "(Ljava/lang/Object;)Z");

		//fill/update map
		for(ConversationThreadList::iterator coreListIter = coreConversationThreads->begin();
				coreListIter != coreConversationThreads->end(); coreListIter++)
		{
			//fetch List item object / OPConversationThread
			jclass conversationThreadClass = findClass("com/openpeer/javaapi/OPConversationThread");
			jmethodID conversationThreadConstructorMethodID = jni_env->GetMethodID(conversationThreadClass, "<init>", "()V");
			jobject conversationThreadObject = jni_env->NewObject(conversationThreadClass, conversationThreadConstructorMethodID);

			//add to map for future calls
			//conversationThreadMap.insert(std::pair<jobject, IConversationThreadPtr>(conversationThreadObject, *coreListIter));

			//add to return List
			jboolean success = jni_env->CallBooleanMethod(returnListObject,listAddMethodID , conversationThreadObject);

		}

	}
	return returnListObject;
}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    getConversationThreadByID
 * Signature: (Lcom/openpeer/javaapi/OPAccount;Ljava/lang/String;)Lcom/openpeer/javaapi/OPConversationThread;
 */
JNIEXPORT jobject JNICALL Java_com_openpeer_javaapi_OPConversationThread_getConversationThreadByID
(JNIEnv *env, jclass, jobject, jstring threadID)
{
	jclass cls;
	jmethodID method;
	jobject object;
	JNIEnv *jni_env = 0;
	IConversationThreadPtr conversationThreadPtr;

	const char *threadIDStr;
	threadIDStr = env->GetStringUTFChars(threadID, NULL);
	if (threadIDStr == NULL) {
		return object;
	}

	//TODO refactor entire method, this list should be kept in upper layer, and never call native method
	if(OpenPeerCoreManager::accountPtr)
	{
		conversationThreadPtr = IConversationThread::getConversationThreadByID(OpenPeerCoreManager::accountPtr, threadIDStr);
	}

	if(conversationThreadPtr)
	{
		jni_env = getEnv();
		if(jni_env)
		{
			cls = findClass("com/openpeer/javaapi/OPConversationThread");
			method = jni_env->GetMethodID(cls, "<init>", "()V");
			object = jni_env->NewObject(cls, method);

			jfieldID fid = jni_env->GetFieldID(cls, "nativeClassPointer", "J");
			jlong convThread = (jlong) conversationThreadPtr.get();
			jni_env->SetLongField(object, fid, convThread);

			__android_log_print(ANDROID_LOG_INFO, "com.openpeer.jni",
					"CorePtr raw = %p, ptr as long = %Lu",conversationThreadPtr.get(), convThread);

			OpenPeerCoreManager::coreConversationThreadList.push_back(conversationThreadPtr);
		}
	}
	return object;
}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    getStableID
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_openpeer_javaapi_OPConversationThread_getStableID
(JNIEnv *, jobject owner)
{
	jlong ret = 0;
	IConversationThreadPtr coreThread = OpenPeerCoreManager::getConversationThreadFromList(owner);
	if (coreThread)
	{
		ret = coreThread->getID();
	}
	return ret;
}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    getThreadID
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_openpeer_javaapi_OPConversationThread_getThreadID
(JNIEnv *env, jobject owner)
{
	jstring ret;
	IConversationThreadPtr coreThread = OpenPeerCoreManager::getConversationThreadFromList(owner);
	if (coreThread)
	{
		ret = env->NewStringUTF(coreThread->getThreadID().c_str());
	}
	return ret;
}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    amIHost
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_openpeer_javaapi_OPConversationThread_amIHost
(JNIEnv *, jobject owner)
{
	jboolean ret;
	IConversationThreadPtr coreThread = OpenPeerCoreManager::getConversationThreadFromList(owner);
	if (coreThread)
	{
		ret = coreThread->amIHost();
	}
	return ret;
}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    getAssociatedAccount
 * Signature: ()Lcom/openpeer/javaapi/OPAccount;
 */
JNIEXPORT jobject JNICALL Java_com_openpeer_javaapi_OPConversationThread_getAssociatedAccount
(JNIEnv *, jobject)
{
	//TODO This should not be native call. Current limitation is single account at the time, so it should return OPaccount object
	return (jobject) OpenPeerCoreManager::accountPtr.get();
}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    getContacts
 * Signature: ()Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_com_openpeer_javaapi_OPConversationThread_getContacts
(JNIEnv *, jobject owner)
{
	jclass cls;
	jmethodID method;
	jobject returnListObject;
	JNIEnv *jni_env = 0;

	//Core Contact list
	ContactListPtr coreContacts;


	//take contacts from core conversation thread
	IConversationThreadPtr coreThread = OpenPeerCoreManager::getConversationThreadFromList(owner);
	if (coreThread)
	{
		coreContacts = coreThread->getContacts();
	}

	//fetch JNI env
	jni_env = getEnv();
	if(jni_env)
	{
		//create return object - java/util/List is interface, ArrayList is implementation
		jclass returnListClass = findClass("java/util/ArrayList");
		jmethodID listConstructorMethodID = jni_env->GetMethodID(returnListClass, "<init>", "()V");
		returnListObject = jni_env->NewObject(returnListClass, listConstructorMethodID);


		//fetch List.add object
		jmethodID listAddMethodID = jni_env->GetMethodID(returnListClass, "add", "(Ljava/lang/Object;)Z");

		//fill/update map
		for(ContactList::iterator coreListIter = coreContacts->begin();
				coreListIter != coreContacts->end(); coreListIter++)
		{
			//fetch List item object / OPContact
			jclass contactClass = findClass("com/openpeer/javaapi/OPContact");
			jmethodID contactConstructorMethodID = jni_env->GetMethodID(contactClass, "<init>", "()V");
			jobject contactObject = jni_env->NewObject(contactClass, contactConstructorMethodID);

			jfieldID fid = jni_env->GetFieldID(contactClass, "nativeClassPointer", "J");
			jlong contact = (jlong) (*coreListIter).get();
			jni_env->SetLongField(contactObject, fid, contact);
			OpenPeerCoreManager::coreContactList.push_back((*coreListIter));

			//add to return List
			jboolean success = jni_env->CallBooleanMethod(returnListObject,listAddMethodID , contactObject);

		}
	}
	return returnListObject;
}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    getIdentityContactList
 * Signature: (Lcom/openpeer/javaapi/OPContact;)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_com_openpeer_javaapi_OPConversationThread_getIdentityContactList
(JNIEnv *, jobject owner, jobject contact)
{
	jclass cls;
	jmethodID method;
	jobject returnListObject;
	jobject object;
	JNIEnv *jni_env = 0;

	IContactPtr contactPtr = OpenPeerCoreManager::getContactFromList(contact);
	IdentityContact coreContact;
	IdentityContactListPtr coreContactList;

	//take contacts from core conversation thread
	IConversationThreadPtr coreThread = OpenPeerCoreManager::getConversationThreadFromList(owner);
	if (coreThread)
	{
		coreContactList = coreThread->getIdentityContactList(contactPtr);

	}
	jni_env = getEnv();
	if(jni_env)
	{
		//create return object - java/util/List is interface, ArrayList is implementation
		jclass returnListClass = findClass("java/util/ArrayList");
		jmethodID listConstructorMethodID = jni_env->GetMethodID(returnListClass, "<init>", "()V");
		returnListObject = jni_env->NewObject(returnListClass, listConstructorMethodID);


		//fetch List.add object
		jmethodID listAddMethodID = jni_env->GetMethodID(returnListClass, "add", "(Ljava/lang/Object;)Z");

		for(IdentityContactList::iterator iter = coreContactList->begin(); iter != coreContactList->end(); iter ++)
		{
			coreContact = *iter;

			cls = findClass("com/openpeer/javaapi/OPIdentityContact");
			method = jni_env->GetMethodID(cls, "<init>", "()V");
			object = jni_env->NewObject(cls, method);

			//set Stable ID to OPIdentityContact
			method = jni_env->GetMethodID(cls, "setStableID", "(Ljava/lang/String;)V");
			jstring stableID =  jni_env->NewStringUTF(coreContact.mStableID.c_str());
			jni_env->CallVoidMethod(object, method, stableID);

			//set Public Peer File to OPIdentityContact
			//TODO export peer file public to ElementPtr and then convert to String
			jclass peerFileCls = findClass("com/openpeer/javaapi/OPPeerFilePublic");
			jmethodID peerFileMethodID = jni_env->GetMethodID(peerFileCls, "<init>", "()V");
			jobject peerFileObject = jni_env->NewObject(peerFileCls, peerFileMethodID);
			method = jni_env->GetMethodID(cls, "setPeerFilePublic", "(Lcom/openpeer/javaapi/OPPeerFilePublic;)V");
			jni_env->CallVoidMethod(object, method, peerFileObject);

			//set IdentityProofBundle to OPIdentityContact
			method = jni_env->GetMethodID(cls, "setIdentityProofBundle", "(Ljava/lang/String;)V");
			jstring identityProofBundle =  jni_env->NewStringUTF(IHelper::convertToString(coreContact.mIdentityProofBundleEl).c_str());
			jni_env->CallVoidMethod(object, method, identityProofBundle);

			//set Priority to OPIdentityContact
			method = jni_env->GetMethodID(cls, "setPriority", "(I)V");
			jni_env->CallVoidMethod(object, method, (int)coreContact.mPriority);

			//set Weight to OPIdentityContact
			method = jni_env->GetMethodID(cls, "setWeight", "(I)V");
			jni_env->CallVoidMethod(object, method, (int)coreContact.mWeight);

			//Convert and set time from C++ to Android; Fetch methods needed to accomplish this
			Time time_t_epoch = boost::posix_time::time_from_string("1970-01-01 00:00:00.000");
			jclass timeCls = findClass("android/text/format/Time");
			jmethodID timeMethodID = jni_env->GetMethodID(timeCls, "<init>", "()V");
			jmethodID timeSetMillisMethodID   = jni_env->GetMethodID(timeCls, "set", "(J)V");

			//calculate and set Last Updated
			zsLib::Duration lastUpdated = coreContact.mLastUpdated - time_t_epoch;
			jobject timeLastUpdatedObject = jni_env->NewObject(timeCls, timeMethodID);
			jni_env->CallVoidMethod(timeLastUpdatedObject, timeSetMillisMethodID, lastUpdated.total_milliseconds());
			//Time has been converted, now call OPIdentityContact setter
			method = jni_env->GetMethodID(cls, "setLastUpdated", "(Landroid/text/format/Time;)V");
			jni_env->CallVoidMethod(object, method, timeLastUpdatedObject);

			//calculate and set Expires
			zsLib::Duration expires = coreContact.mExpires - time_t_epoch;
			jobject timeExpiresObject = jni_env->NewObject(timeCls, timeMethodID);
			jni_env->CallVoidMethod(timeExpiresObject, timeSetMillisMethodID, expires.total_milliseconds());
			//Time has been converted, now call OPIdentityContact setter
			method = jni_env->GetMethodID(cls, "setExpires", "(Landroid/text/format/Time;)V");
			jni_env->CallVoidMethod(object, method, timeExpiresObject);


			///////////////////////////////////////////////////////////////
			// SET ROLODEX CONTACT FIELDS
			//////////////////////////////////////////////////////////////

			//Fetch setDisposition method from OPDownloadedRolodexContacts class
			//jclass dispositionClass = findClass("com/openpeer/javaapi/OPRolodexContact$Dispositions");
			//jmethodID dispositionConstructorMethodID = jni_env->GetMethodID(cls, "<init>", "()V");
			jmethodID setDispositionMethodID = jni_env->GetMethodID( cls, "setDisposition", "(Lcom/openpeer/javaapi/OPRolodexContact$Dispositions;)V" );
			//Fetch setIdentityURI method from OPDownloadedRolodexContacts class
			jmethodID setIdentityURIMethodID = jni_env->GetMethodID( cls, "setIdentityURI", "(Ljava/lang/String;)V" );
			//Fetch setIdentityProvider method from OPDownloadedRolodexContacts class
			jmethodID setIdentityProviderMethodID = jni_env->GetMethodID( cls, "setIdentityProvider", "(Ljava/lang/String;)V" );
			//Fetch setName method from OPDownloadedRolodexContacts class
			jmethodID setNameMethodID = jni_env->GetMethodID( cls, "setName", "(Ljava/lang/String;)V" );
			//Fetch setProfileURL method from OPDownloadedRolodexContacts class
			jmethodID setProfileURLMethodID = jni_env->GetMethodID( cls, "setProfileURL", "(Ljava/lang/String;)V" );
			//Fetch setVProfileURL method from OPDownloadedRolodexContacts class
			jmethodID setVProfileURLMethodID = jni_env->GetMethodID( cls, "setVProfileURL", "(Ljava/lang/String;)V" );
			//Fetch setAvatars method from OPDownloadedRolodexContacts class
			jmethodID setAvatarsMethodID = jni_env->GetMethodID( cls, "setAvatars", "(Ljava/util/List;)V");


			//avatar list fetch
			jclass avatarListClass = findClass("java/util/ArrayList");
			jmethodID avatarListConstructorMethodID = jni_env->GetMethodID(avatarListClass, "<init>", "()V");
			jobject avatarListObject = jni_env->NewObject(avatarListClass, avatarListConstructorMethodID);
			jmethodID avatarListAddMethodID = jni_env->GetMethodID(avatarListClass, "add", "(Ljava/lang/Object;)Z");


			//OPAvatar class and methods fetch
			jclass avatarClass = findClass("com/openpeer/javaapi/OPRolodexContact$OPAvatar");
			jmethodID avatarConstructorMethodID = jni_env->GetMethodID(avatarClass, "<init>", "(Lcom/openpeer/javaapi/OPRolodexContact;)V");
			jmethodID setAvatarNameMethodID = jni_env->GetMethodID(avatarClass, "setName", "(Ljava/lang/String;)V");
			jmethodID setAvatarURLMethodID = jni_env->GetMethodID(avatarClass, "setURL", "(Ljava/lang/String;)V");
			jmethodID setAvatarWidthMethodID = jni_env->GetMethodID(avatarClass, "setWidth", "(I)V");
			jmethodID setAvatarHeightMethodID = jni_env->GetMethodID(avatarClass, "setHeight", "(I)V");

			//set Disposition to OPRolodexContact
			jobject dispositionObject = OpenPeerCoreManager::getJavaEnumObject("com/openpeer/javaapi/OPRolodexContact$Dispositions", (jint)coreContact.mDisposition);
			jni_env->CallVoidMethod(object, setDispositionMethodID, dispositionObject);

			//set identity URI to OPRolodexContact
			jstring identityUriStr = jni_env->NewStringUTF(coreContact.mIdentityURI.c_str());
			jni_env->CallVoidMethod(object, setIdentityURIMethodID, identityUriStr);

			//set identity provider to OPRolodexContact
			jstring identityProviderStr = jni_env->NewStringUTF(coreContact.mIdentityProvider.c_str());
			jni_env->CallVoidMethod(object, setIdentityProviderMethodID, identityProviderStr);

			//set name to OPRolodexContact
			jstring nameStr = jni_env->NewStringUTF(coreContact.mName.c_str());
			jni_env->CallVoidMethod(object, setNameMethodID, nameStr);

			//set profile URL to OPRolodexContact
			jstring profileURLStr = jni_env->NewStringUTF(coreContact.mProfileURL.c_str());
			jni_env->CallVoidMethod(object, setProfileURLMethodID, profileURLStr);

			//set v profile URL to OPRolodexContact
			jstring vProfileURLStr = jni_env->NewStringUTF(coreContact.mVProfileURL.c_str());
			jni_env->CallVoidMethod(object, setVProfileURLMethodID, vProfileURLStr);

			//set avatars to OPAvatarList
			for (RolodexContact::AvatarList::iterator avatarIter = coreContact.mAvatars.begin();
					avatarIter != coreContact.mAvatars.end(); avatarIter++)
			{
				RolodexContact::Avatar coreAvatar = *avatarIter;
				//create OPAvatar object
				jobject avatarObject = jni_env->NewObject(avatarClass, avatarConstructorMethodID);

				//set avatar name to OPRolodexContact::OPAvatar
				jstring avatarNameStr = jni_env->NewStringUTF(coreAvatar.mName.c_str());
				jni_env->CallVoidMethod(avatarObject, setAvatarNameMethodID, avatarNameStr);

				//set avatar URL to OPRolodexContact::OPAvatar
				jstring avatarURLStr = jni_env->NewStringUTF(coreAvatar.mURL.c_str());
				jni_env->CallVoidMethod(avatarObject, setAvatarURLMethodID, avatarURLStr);

				//set avatar width to OPRolodexContact::OPAvatar
				jni_env->CallVoidMethod(avatarObject, setAvatarWidthMethodID, (jint)coreAvatar.mWidth);

				//set avatar height to OPRolodexContact::OPAvatar
				jni_env->CallVoidMethod(avatarObject, setAvatarHeightMethodID, (jint)coreAvatar.mHeight);

				//add avatar object to avatar list
				jboolean success = jni_env->CallBooleanMethod(avatarListObject, avatarListAddMethodID , avatarObject);
			}

			//add avatar list to OPRolodexContact
			jni_env->CallVoidMethod(object, setAvatarsMethodID, avatarListObject);

			//add to return List
			jboolean success = jni_env->CallBooleanMethod(returnListObject,listAddMethodID , object);

		}

	}
	return returnListObject;
}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    getContactState
 * Signature: (Lcom/openpeer/javaapi/OPContact;)Lcom/openpeer/javaapi/ContactStates;
 */
JNIEXPORT jobject JNICALL Java_com_openpeer_javaapi_OPConversationThread_getContactState
(JNIEnv *, jobject owner, jobject contact)
{
	jclass cls;
	jmethodID method;
	jobject object;
	JNIEnv *jni_env = 0;
	jint state = 0;

	IContactPtr coreContact = OpenPeerCoreManager::getContactFromList(contact);
	IConversationThreadPtr coreThread = OpenPeerCoreManager::getConversationThreadFromList(owner);
	if (coreThread &&coreContact)
	{
		state = (jint) coreThread->getContactState(coreContact);
		jni_env = getEnv();
		if(jni_env)
		{
			object = OpenPeerCoreManager::getJavaEnumObject("com/openpeer/javaapi/ContactStates", state);

		}
	}
	return object;
}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    addContacts
 * Signature: (Ljava/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_openpeer_javaapi_OPConversationThread_addContacts
(JNIEnv *, jobject owner, jobject contactProfileInfos)
{
	jclass cls;
	jmethodID method;
	jobject object;
	JNIEnv *jni_env = 0;

	//Core contact profile list
	ContactProfileInfoList coreContactProfilesToAdd;

	//fetch JNI env
	jni_env = getEnv();
	if(jni_env)
	{
		//create return object - java/util/List is interface, ArrayList is implementation
		jclass arrayListClass = findClass("java/util/ArrayList");
		if(jni_env->IsInstanceOf(contactProfileInfos, arrayListClass) != JNI_TRUE)
		{
			return;
		}
		// Fetch "java.util.List.get(int location)" MethodID
		jmethodID listGetMethodID = jni_env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");
		// Fetch "int java.util.List.size()" MethodID
		jmethodID sizeMethodID = jni_env->GetMethodID( arrayListClass, "size", "()I" );

		// Call "int java.util.List.size()" method and get count of items in the list.
		int listItemsCount = (int)jni_env->CallIntMethod( contactProfileInfos, sizeMethodID );

		for( int i=0; i<listItemsCount; ++i )
		{
			// Call "java.util.List.get" method and get Contact object by index.
			jobject contactProfileInfoObject = jni_env->CallObjectMethod( contactProfileInfos, listGetMethodID, i );
			if( contactProfileInfoObject != NULL )
			{
				//Fetch OPContactProfileInfo class
				jclass contactProfileInfoClass = findClass("com/openpeer/javaapi/OPContactProfileInfo");
				//Fetch getContact method from OPContactProfileInfo class
				jmethodID getContactMethodID = jni_env->GetMethodID( contactProfileInfoClass, "getContact", "()Lcom/openpeer/javaapi/OPContact;" );

				// Call "getContact method to fetch contact from Contact profile info
				jobject contactObject = jni_env->CallObjectMethod( contactProfileInfoObject, getContactMethodID );
				IContactPtr contact = OpenPeerCoreManager::getContactFromList(contactObject);
				ContactProfileInfo contactProfileInfo;
				contactProfileInfo.mContact = contact;
				//todo add profile bundle to contact profile info

				//add core contacts to list for removal
				coreContactProfilesToAdd.push_front(contactProfileInfo);

			}
		}
	}
	//remove contacts from conversation thread
	IConversationThreadPtr coreThread = OpenPeerCoreManager::getConversationThreadFromList(owner);
	if (coreThread)
	{
		coreThread->addContacts(coreContactProfilesToAdd);
	}
}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    removeContacts
 * Signature: (Ljava/util/List;)V
 */
JNIEXPORT void JNICALL Java_com_openpeer_javaapi_OPConversationThread_removeContacts
(JNIEnv *, jobject owner, jobject contactsToRemove)
{
	jclass cls;
	jmethodID method;
	jobject object;
	JNIEnv *jni_env = 0;

	//Core contact list
	ContactList coreContactsToRemove;

	//fetch JNI env
	jni_env = getEnv();
	if(jni_env)
	{
		//create return object - java/util/List is interface, ArrayList is implementation
		jclass arrayListClass = findClass("java/util/ArrayList");
		if(jni_env->IsInstanceOf(contactsToRemove, arrayListClass) != JNI_TRUE)
		{
			return;
		}
		// Fetch "java.util.List.get(int location)" MethodID
		jmethodID listGetMethodID = jni_env->GetMethodID(arrayListClass, "get", "(I)Ljava/lang/Object;");
		// Fetch "int java.util.List.size()" MethodID
		jmethodID sizeMethodID = jni_env->GetMethodID( arrayListClass, "size", "()I" );

		// Call "int java.util.List.size()" method and get count of items in the list.
		int listItemsCount = (int)jni_env->CallIntMethod( contactsToRemove, sizeMethodID );

		for( int i=0; i<listItemsCount; ++i )
		{
			// Call "java.util.List.get" method and get Contact object by index.
			jobject contactObject = jni_env->CallObjectMethod( contactsToRemove, listGetMethodID, i);
			if( contactObject != NULL )
			{
				IContactPtr contact = contactMap.find(contactObject)->second;
				//add core contacts to list for removal
				coreContactsToRemove.push_front(contact);
				//remove contact entry from jni contact map
				contactMap.erase(contactObject);
			}
		}
	}
	//remove contacts from conversation thread
	IConversationThreadPtr coreThread = OpenPeerCoreManager::getConversationThreadFromList(owner);
	if (coreThread)
	{
		coreThread->removeContacts(coreContactsToRemove);
	}
}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    sendMessage
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_openpeer_javaapi_OPConversationThread_sendMessage
(JNIEnv *env, jobject owner, jstring messageID, jstring messageType, jstring message)
{
	const char *messageIDStr;
	messageIDStr = env->GetStringUTFChars(messageID, NULL);
	if (messageIDStr == NULL) {
		return;
	}

	const char *messageTypeStr;
	messageTypeStr = env->GetStringUTFChars(messageType, NULL);
	if (messageTypeStr == NULL) {
		return;
	}

	const char *messageStr;
	messageStr = env->GetStringUTFChars(message, NULL);
	if (messageStr == NULL) {
		return;
	}

	IConversationThreadPtr coreThread = OpenPeerCoreManager::getConversationThreadFromList(owner);
	if (coreThread)
	{
		coreThread->sendMessage(messageIDStr, messageTypeStr, messageStr, true);
	}
}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    getMessage
 * Signature: (Ljava/lang/String;)Lcom/openpeer/javaapi/OPMessage;
 */
JNIEXPORT jobject JNICALL Java_com_openpeer_javaapi_OPConversationThread_getMessage
(JNIEnv *env, jobject owner, jstring messageID)
{
	jclass cls;
	jmethodID method;
	jobject messageObject;
	JNIEnv *jni_env = 0;

	const char *messageIDStr;
	messageIDStr = env->GetStringUTFChars(messageID, NULL);
	if (messageIDStr == NULL) {
		return NULL;
	}

	IConversationThreadPtr coreThread = OpenPeerCoreManager::getConversationThreadFromList(owner);
	if (coreThread)
	{
		jni_env = getEnv();
		if(jni_env)
		{
			IContactPtr outFrom;
			String outMessageType;
			String outMessage;
			Time outTime;

			jboolean exists = coreThread->getMessage(messageIDStr, outFrom, outMessageType, outMessage, outTime);
			if (exists)
			{
				//Convert out parameters from c++ to return object for java
				//Create OPMessage object
				jclass messageClass = findClass("com/openpeer/javaapi/OPMessage");
				jmethodID messageConstructorMethodID = jni_env->GetMethodID(messageClass, "<init>", "()V");
				messageObject = jni_env->NewObject(messageClass, messageConstructorMethodID);

				//FETCH METHODS TO GET INFO FROM JAVA
				//Fetch setFrom method from OPMessage class
				jmethodID setFromMethodID = jni_env->GetMethodID( messageClass, "setFrom", "(Lcom/openpeer/javaapi/OPPeerFilePublic;)V" );
				//Fetch setMessageType method from OPMessage class
				jmethodID setMessageTypeMethodID = jni_env->GetMethodID( messageClass, "setMessageType", "(Ljava/lang/String;)V" );
				//Fetch setMessage method from OPMessage class
				jmethodID setMessageMethodID = jni_env->GetMethodID( messageClass, "setMessage", "(Ljava/lang/String;)V" );
				//Fetch setTime method from OPMessage class
				jmethodID setTimeMethodID = jni_env->GetMethodID( messageClass, "setTime", "(Landroid/text/format/Time;)V" );

				//Convert parameter and call setFrom method on return object
				jobject from;
				for(std::map<jobject, IContactPtr>::iterator iter = contactMap.begin(); iter != contactMap.end(); ++iter)
				{
					if (iter->second == outFrom)
					{
						from = iter->first;
						break;
					}
				}
				jni_env->CallVoidMethod( messageObject, setFromMethodID, from );

				//Convert parameter and call setMessageType method on return object
				jstring messageType = jni_env->NewStringUTF(outMessageType.c_str());
				jni_env->CallVoidMethod( messageObject, setMessageTypeMethodID, messageType );

				//Convert parameter and call setMessage method on return object
				jstring message = jni_env->NewStringUTF(outMessage.c_str());
				jni_env->CallVoidMethod( messageObject, setMessageMethodID, message );


				//Convert parameter and call setTime method on return object
				//Convert and set time from C++ to Android; Fetch methods needed to accomplish this
				Time time_t_epoch = boost::posix_time::time_from_string("1970-01-01 00:00:00.000");
				jclass timeCls = findClass("android/text/format/Time");
				jmethodID timeMethodID = jni_env->GetMethodID(timeCls, "<init>", "()V");
				jmethodID timeSetMillisMethodID   = jni_env->GetMethodID(timeCls, "set", "(J)V");

				//calculate and set time
				zsLib::Duration timeDuration = outTime - time_t_epoch;
				jobject timeObject = jni_env->NewObject(timeCls, timeMethodID);
				jni_env->CallVoidMethod(timeObject, timeSetMillisMethodID, timeDuration.total_milliseconds());

				//set time to OPMessage
				jni_env->CallVoidMethod( messageObject, setTimeMethodID, timeObject );
			}
		}
	}
	return messageObject;
}

/*
 * Class:     com_openpeer_javaapi_OPConversationThread
 * Method:    getMessageDeliveryState
 * Signature: (Ljava/lang/String;)Lcom/openpeer/javaapi/MessageDeliveryStates;
 */
JNIEXPORT jobject JNICALL Java_com_openpeer_javaapi_OPConversationThread_getMessageDeliveryState
(JNIEnv *env, jobject owner, jstring messageID)
{
	jclass cls;
	jmethodID method;
	jobject object;
	JNIEnv *jni_env = 0;

	const char *messageIDStr;
	messageIDStr = env->GetStringUTFChars(messageID, NULL);
	if (messageIDStr == NULL) {
		return NULL;
	}

	IConversationThreadPtr coreThread = OpenPeerCoreManager::getConversationThreadFromList(owner);
	if (coreThread)
	{
		jni_env = getEnv();
		if(jni_env)
		{
			IConversationThread::MessageDeliveryStates stateValue;
			jboolean exists = coreThread->getMessageDeliveryState(messageIDStr, stateValue);
			if (exists)
			{
				object = OpenPeerCoreManager::getJavaEnumObject("com/openpeer/javaapi/OPMessageDeliveryStates", (jint)stateValue);
			}
			else
			{
				return NULL;
			}
		}
	}
	return object;
}

#ifdef __cplusplus
}
#endif
