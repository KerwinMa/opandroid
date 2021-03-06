#include "IdentityDelegateWrapper.h"
#include "OpenPeerCoreManager.h"
#include "android/log.h"
#include "globals.h"


//IIdentityDelegate implementation
IdentityDelegateWrapper::IdentityDelegateWrapper(jobject delegate)
{
	JNIEnv *jni_env = getEnv();
	javaDelegate = jni_env->NewGlobalRef(delegate);
}


void IdentityDelegateWrapper::onIdentityStateChanged(
		IIdentityPtr identity,
		IIdentity::IdentityStates state
)
{
	jclass cls;
	jmethodID method;
	jobject object;
	JNIEnv *jni_env = 0;

	__android_log_print(ANDROID_LOG_DEBUG, "com.openpeer.jni", "onIdentityStateChanged state = %d", (jint)state);

	bool attached = false;
	switch (android_jvm->GetEnv((void**)&jni_env, JNI_VERSION_1_6))
	{
	case JNI_OK:
		break;
	case JNI_EDETACHED:
		if (android_jvm->AttachCurrentThread(&jni_env, NULL)!=0)
		{
			throw std::runtime_error("Could not attach current thread");
		}
		attached = true;
		break;
	case JNI_EVERSION:
		throw std::runtime_error("Invalid java version");
	}
	if (javaDelegate != NULL)
	{
		//create new OPCall java object
		cls = findClass("com/openpeer/javaapi/OPIdentity");
		method = jni_env->GetMethodID(cls, "<init>", "()V");
		jobject identityObject = jni_env->NewObject(cls, method);

		//fill new field with pointer to core pointer
		IIdentityPtr* ptrToIdentity = new boost::shared_ptr<IIdentity>(identity);
		jfieldID fid = jni_env->GetFieldID(cls, "nativeClassPointer", "J");
		jni_env->SetLongField(identityObject, fid, (jlong)ptrToIdentity);

		//get delegate implementation class name in order to get method
		String className = OpenPeerCoreManager::getObjectClassName(javaDelegate);

		jclass callbackClass = findClass(className.c_str());
		method = jni_env->GetMethodID(callbackClass, "onIdentityStateChanged", "(Lcom/openpeer/javaapi/OPIdentity;Lcom/openpeer/javaapi/IdentityStates;)V");
		jni_env->CallVoidMethod(javaDelegate, method, identityObject, OpenPeerCoreManager::getJavaEnumObject("com/openpeer/javaapi/IdentityStates", (jint) state));
	}
	else
	{
		__android_log_print(ANDROID_LOG_ERROR, "com.openpeer.jni", "onIdentityStateChanged Java delegate is NULL !!!");
	}
	if (jni_env->ExceptionCheck()) {
		jni_env->ExceptionDescribe();
	}

	if(attached)
	{
		android_jvm->DetachCurrentThread();
	}
}
void IdentityDelegateWrapper::onIdentityPendingMessageForInnerBrowserWindowFrame(IIdentityPtr identity)
{
	jclass cls;
	jmethodID method;
	jobject object;
	JNIEnv *jni_env = 0;

	__android_log_print(ANDROID_LOG_DEBUG, "com.openpeer.jni", "onIdentityPendingMessageForInnerBrowserWindowFrame called");

	bool attached = false;
	switch (android_jvm->GetEnv((void**)&jni_env, JNI_VERSION_1_6))
	{
	case JNI_OK:
		break;
	case JNI_EDETACHED:
		if (android_jvm->AttachCurrentThread(&jni_env, NULL)!=0)
		{
			throw std::runtime_error("Could not attach current thread");
		}
		attached = true;
		break;
	case JNI_EVERSION:
		throw std::runtime_error("Invalid java version");
	}
	if (javaDelegate != NULL)
	{
		//create new OPCall java object
		cls = findClass("com/openpeer/javaapi/OPIdentity");
		method = jni_env->GetMethodID(cls, "<init>", "()V");
		jobject identityObject = jni_env->NewObject(cls, method);

		//fill new field with pointer to core pointer
		IIdentityPtr* ptrToIdentity = new boost::shared_ptr<IIdentity>(identity);
		jfieldID fid = jni_env->GetFieldID(cls, "nativeClassPointer", "J");
		jni_env->SetLongField(identityObject, fid, (jlong)ptrToIdentity);

		//get delegate implementation class name in order to get method
		String className = OpenPeerCoreManager::getObjectClassName(javaDelegate);

		jclass callbackClass = findClass(className.c_str());
		method = jni_env->GetMethodID(callbackClass, "onIdentityPendingMessageForInnerBrowserWindowFrame", "(Lcom/openpeer/javaapi/OPIdentity;)V");
		jni_env->CallVoidMethod(javaDelegate, method, identityObject);
	}
	else
	{
		__android_log_print(ANDROID_LOG_ERROR, "com.openpeer.jni", "onIdentityPendingMessageForInnerBrowserWindowFrame Java delegate is NULL !!!");
	}
	if (jni_env->ExceptionCheck()) {
		jni_env->ExceptionDescribe();
	}

	if(attached)
	{
		android_jvm->DetachCurrentThread();
	}
}
void IdentityDelegateWrapper::onIdentityRolodexContactsDownloaded(IIdentityPtr identity)
{
	jclass cls;
	jmethodID method;
	jobject object;
	JNIEnv *jni_env = 0;

	__android_log_print(ANDROID_LOG_DEBUG, "com.openpeer.jni", "onIdentityRolodexContactsDownloaded called");

	bool attached = false;
	switch (android_jvm->GetEnv((void**)&jni_env, JNI_VERSION_1_6))
	{
	case JNI_OK:
		break;
	case JNI_EDETACHED:
		if (android_jvm->AttachCurrentThread(&jni_env, NULL)!=0)
		{
			throw std::runtime_error("Could not attach current thread");
		}
		attached = true;
		break;
	case JNI_EVERSION:
		throw std::runtime_error("Invalid java version");
	}
	if (javaDelegate != NULL)
	{
		//create new OPCall java object
		cls = findClass("com/openpeer/javaapi/OPIdentity");
		method = jni_env->GetMethodID(cls, "<init>", "()V");
		jobject identityObject = jni_env->NewObject(cls, method);

		//fill new field with pointer to core pointer
		IIdentityPtr* ptrToIdentity = new boost::shared_ptr<IIdentity>(identity);
		jfieldID fid = jni_env->GetFieldID(cls, "nativeClassPointer", "J");
		jni_env->SetLongField(identityObject, fid, (jlong)ptrToIdentity);

		//get delegate implementation class name in order to get method
		String className = OpenPeerCoreManager::getObjectClassName(javaDelegate);

		jclass callbackClass = findClass(className.c_str());
		method = jni_env->GetMethodID(callbackClass, "onIdentityRolodexContactsDownloaded", "(Lcom/openpeer/javaapi/OPIdentity;)V");
		jni_env->CallVoidMethod(javaDelegate, method, identityObject);
	}
	else
	{
		__android_log_print(ANDROID_LOG_ERROR, "com.openpeer.jni", "onIdentityRolodexContactsDownloaded Java delegate is NULL !!!");
	}
	if (jni_env->ExceptionCheck()) {
		jni_env->ExceptionDescribe();
	}

	if(attached)
	{
		android_jvm->DetachCurrentThread();
	}
}

IdentityDelegateWrapper::~IdentityDelegateWrapper()
{
	JNIEnv *jni_env = getEnv();
	jni_env->DeleteGlobalRef(javaDelegate);

}
