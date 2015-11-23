#pragma once	

#include <jni.h>
#include <memory>

class JniHelpers
{
public:
	static JniHelpers* get();

public:
	void setJavaVM(JavaVM* javaVM);
	void setEnv(JNIEnv* env);

	jmethodID getMethodID(const char* clazz, const char* method, const char* signatue);
	std::string jstring2string(jstring str);

	JavaVM* getJavaVM();
	JNIEnv* getEnv();

private:
	JavaVM* m_javaVM = nullptr;
	JNIEnv* m_env = nullptr;

private:
	JniHelpers() = default;
	static JniHelpers* m_instance;
};