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
//   https://github.com/apache/hadoop/blob/release-2.7.1/hadoop-hdfs-project/hadoop-hdfs/src/main/native/libhdfs/os/posix/mutexes.c
// We need Hadoop 2.7 for Hortonworks HDP support, but libhdfs no longer exports
// getJNIEnv so we have to copy some source files like this to Impala. ROCANA-3465
// See these files also: exception.c/.h htable.c/.h jni_helper.c/.h mutexes.c/.h thread_local_storage.c/.h
// In this file we only changed the path in one of the #includes (and added this comment block).

#include "mutexes.h"

#include <pthread.h>
#include <stdio.h>

mutex hdfsHashMutex = PTHREAD_MUTEX_INITIALIZER;
mutex jvmMutex = PTHREAD_MUTEX_INITIALIZER;

int mutexLock(mutex *m) {
  int ret = pthread_mutex_lock(m);
  if (ret) {
    fprintf(stderr, "mutexLock: pthread_mutex_lock failed with error %d\n",
      ret);
  }
  return ret;
}

int mutexUnlock(mutex *m) {
  int ret = pthread_mutex_unlock(m);
  if (ret) {
    fprintf(stderr, "mutexUnlock: pthread_mutex_unlock failed with error %d\n",
      ret);
  }
  return ret;
}
