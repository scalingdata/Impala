/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Copied from
//   https://github.com/apache/hadoop/blob/release-2.7.1/hadoop-hdfs-project/hadoop-hdfs/src/main/native/libhdfs/jni_helper.h
// We need Hadoop 2.7 for Hortonworks HDP support, but libhdfs no longer exports
// getJNIEnv so we have to copy some source files like this to Impala. ROCANA-3465
// See these files also: exception.c/.h htable.c/.h jni_helper.c/.h mutexes.c/.h thread_local_storage.c/.h
// In this file we changed some #includes, removed a #define we didn't need, and removed some methods we didn't
// need (and added this comment block).

#ifndef LIBHDFS_JNI_HELPER_H
#define LIBHDFS_JNI_HELPER_H

#include <jni.h>

/** Denote the method we want to invoke as STATIC or INSTANCE */
typedef enum {
    STATIC,
    INSTANCE
} MethType;

/**
 * Helper function to destroy a local reference of java.lang.Object
 * @param env: The JNIEnv pointer. 
 * @param jFile: The local reference of java.lang.Object object
 * @return None.
 */
void destroyLocalReference(JNIEnv *env, jobject jObject);

/** invokeMethod: Invoke a Static or Instance method.
 * className: Name of the class where the method can be found
 * methName: Name of the method
 * methSignature: the signature of the method "(arg-types)ret-type"
 * methType: The type of the method (STATIC or INSTANCE)
 * instObj: Required if the methType is INSTANCE. The object to invoke
   the method on.
 * env: The JNIEnv pointer
 * retval: The pointer to a union type which will contain the result of the
   method invocation, e.g. if the method returns an Object, retval will be
   set to that, if the method returns boolean, retval will be set to the
   value (JNI_TRUE or JNI_FALSE), etc.
 * exc: If the methods throws any exception, this will contain the reference
 * Arguments (the method arguments) must be passed after methSignature
 * RETURNS: -1 on error and 0 on success. If -1 is returned, exc will have 
   a valid exception reference, and the result stored at retval is undefined.
 */
jthrowable invokeMethod(JNIEnv *env, jvalue *retval, MethType methType,
                 jobject instObj, const char *className, const char *methName, 
                 const char *methSignature, ...);

jthrowable constructNewObjectOfClass(JNIEnv *env, jobject *out, const char *className, 
                                  const char *ctorSignature, ...);

jthrowable methodIdFromClass(const char *className, const char *methName, 
                            const char *methSignature, MethType methType, 
                            JNIEnv *env, jmethodID *out);

jthrowable globalClassReference(const char *className, JNIEnv *env, jclass *out);

/** classNameOfObject: Get an object's class name.
 * @param jobj: The object.
 * @param env: The JNIEnv pointer.
 * @param name: (out param) On success, will contain a string containing the
 * class name. This string must be freed by the caller.
 * @return NULL on success, or the exception
 */
jthrowable classNameOfObject(jobject jobj, JNIEnv *env, char **name);

/** getJNIEnv: A helper function to get the JNIEnv* for the given thread.
 * If no JVM exists, then one will be created. JVM command line arguments
 * are obtained from the LIBHDFS_OPTS environment variable.
 * @param: None.
 * @return The JNIEnv* corresponding to the thread.
 * */
JNIEnv* getJNIEnv(void);

#endif
