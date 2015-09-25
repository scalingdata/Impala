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
//   https://github.com/apache/hadoop/blob/release-2.7.1/hadoop-hdfs-project/hadoop-hdfs/src/main/native/libhdfs/os/mutexes.h
// We need Hadoop 2.7 for Hortonworks HDP support, but libhdfs no longer exports
// getJNIEnv so we have to copy some source files like this to Impala. ROCANA-3465
// See these files also: exception.c/.h htable.c/.h jni_helper.c/.h mutexes.c/.h thread_local_storage.c/.h
// The only thing we changed in this file is to add this comment block.

#ifndef LIBHDFS_MUTEXES_H
#define LIBHDFS_MUTEXES_H

/*
 * Defines abstraction over platform-specific mutexes.  libhdfs has no formal
 * initialization function that users would call from a single-threaded context
 * to initialize the library.  This creates a challenge for bootstrapping the
 * mutexes.  To address this, all required mutexes are pre-defined here with
 * external storage.  Platform-specific implementations must guarantee that the
 * mutexes are initialized via static initialization.
 */

#include "platform.h"

/** Mutex protecting the class reference hash table. */
extern mutex hdfsHashMutex;

/** Mutex protecting singleton JVM instance. */
extern mutex jvmMutex;

/**
 * Locks a mutex.
 *
 * @param m mutex
 * @return 0 if successful, non-zero otherwise
 */
int mutexLock(mutex *m);

/**
 * Unlocks a mutex.
 *
 * @param m mutex
 * @return 0 if successful, non-zero otherwise
 */
int mutexUnlock(mutex *m);

#endif

