/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file  ap_config_layout.h
 * @brief Apache Config Layout
 */

#ifndef AP_CONFIG_LAYOUT_H
#define AP_CONFIG_LAYOUT_H

/* Configured Apache directory layout */
#define DEFAULT_PREFIX "/"
#define DEFAULT_EXP_EXEC_PREFIX "/"
#define DEFAULT_REL_EXEC_PREFIX ""
#define DEFAULT_EXP_BINDIR "//bin"
#define DEFAULT_REL_BINDIR "bin"
#define DEFAULT_EXP_SBINDIR "//sbin"
#define DEFAULT_REL_SBINDIR "sbin"
#define DEFAULT_EXP_LIBEXECDIR "//lib"
#define DEFAULT_REL_LIBEXECDIR "lib"
#define DEFAULT_EXP_MANDIR "//share/man"
#define DEFAULT_REL_MANDIR "share/man"
#define DEFAULT_EXP_SYSCONFDIR "//etc/apache"
#define DEFAULT_REL_SYSCONFDIR "etc/apache"
#define DEFAULT_EXP_DATADIR "/var/www/html"
#define DEFAULT_REL_DATADIR "var/www/html"
#define DEFAULT_EXP_INSTALLBUILDDIR "//share/apache/build"
#define DEFAULT_REL_INSTALLBUILDDIR "share/apache/build"
#define DEFAULT_EXP_ERRORDIR "/var/www/html/error"
#define DEFAULT_REL_ERRORDIR "var/www/html/error"
#define DEFAULT_EXP_ICONSDIR "/var/www/html/icons"
#define DEFAULT_REL_ICONSDIR "var/www/html/icons"
#define DEFAULT_EXP_HTDOCSDIR "/var/www/html/htdocs"
#define DEFAULT_REL_HTDOCSDIR "var/www/html/htdocs"
#define DEFAULT_EXP_MANUALDIR "/var/www/html/manual"
#define DEFAULT_REL_MANUALDIR "var/www/html/manual"
#define DEFAULT_EXP_CGIDIR "/var/www/html/cgi-bin"
#define DEFAULT_REL_CGIDIR "var/www/html/cgi-bin"
#define DEFAULT_EXP_INCLUDEDIR "//include/apache"
#define DEFAULT_REL_INCLUDEDIR "include/apache"
#define DEFAULT_EXP_LOCALSTATEDIR "/var"
#define DEFAULT_REL_LOCALSTATEDIR "var"
#define DEFAULT_EXP_RUNTIMEDIR "/var/run"
#define DEFAULT_REL_RUNTIMEDIR "var/run"
#define DEFAULT_EXP_LOGFILEDIR "/var/log"
#define DEFAULT_REL_LOGFILEDIR "var/log"
#define DEFAULT_EXP_PROXYCACHEDIR "/var/proxy"
#define DEFAULT_REL_PROXYCACHEDIR "var/proxy"
#define DEFAULT_PIDLOG "/var/run/apache2.pid"

#endif /* AP_CONFIG_LAYOUT_H */
