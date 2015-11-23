#include "JniHelpers.h"
#include "SimpleEngine.h"
#include "utils.h"
#include "assert.h"

JniHelpers* JniHelpers::m_instance = nullptr;

JniHelpers* JniHelpers::get()
{
	if (not m_instance)
	{
		m_instance = new JniHelpers();
	}
	return m_instance;
}

void JniHelpers::setJavaVM(JavaVM* javaVM)
{
	m_javaVM = javaVM;
}

void JniHelpers::setEnv(JNIEnv* env)
{
	m_env = env;	
}

JavaVM* JniHelpers::getJavaVM()
{
	return m_javaVM;
}

JNIEnv* JniHelpers::getEnv()
{
	return m_env;
}

jmethodID JniHelpers::getMethodID(const char* className, const char* methodName, const char* signature)
{
	jclass clazz = m_env->FindClass(className);
	if (clazz == 0)
	{
		LOGE("Failed to fetch java class! Name: %s", className);
		return nullptr;
	}
	
    jmethodID method = m_env->GetMethodID(clazz, methodName, signature);
    m_env->DeleteLocalRef(clazz);

    if (method == 0)
	{
		LOGE("Failed to fetch java mathod! Name: %s, Signature: %s", className, signature);
		return nullptr;
	}
    
    return method;
}

std::string JniHelpers::jstring2string(jstring jstr)
{
	const char *buffer = m_env->GetStringUTFChars(jstr, NULL);
    return std::string(buffer);
}