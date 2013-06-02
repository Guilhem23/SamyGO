/*
 * Copyright (C) 2002, 2003, 2004, 2005 Free Software Foundation
 *
 * Author: Nikos Mavroyanopoulos
 *
 * This file is part of GNUTLS-EXTRA.
 *
 * GNUTLS-EXTRA is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * GNUTLS-EXTRA is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNUTLS-EXTRA; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */

/* Note the libgnutls-extra is not a standalone library. It requires
 * to link also against libgnutls.
 */

#ifndef GNUTLS_EXTRA_H
# define GNUTLS_EXTRA_H

#include <gnutls/gnutls.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIBGNUTLS_EXTRA_VERSION LIBGNUTLS_VERSION

/* Openpgp certificate stuff 
 */

/**
 * gnutls_openpgp_recv_key_func - Callback prototype to get OpenPGP keys
 * @session: a TLS session
 * @keyfpr: key fingerprint
 * @keyfpr_length: length of key fingerprint
 * @key: output key.
 *
 * A callback of this type is used to retrieve OpenPGP keys.  Only
 * useful on the server, and will only be used if the peer send a key
 * fingerprint instead of a full key.  See also
 * gnutls_openpgp_set_recv_key_function().
 *
 */
typedef int (*gnutls_openpgp_recv_key_func) (gnutls_session_t session,
					     const unsigned char *keyfpr,
					     unsigned int keyfpr_length,
					     gnutls_datum_t *key);

void gnutls_openpgp_set_recv_key_function( gnutls_session_t session,
					   gnutls_openpgp_recv_key_func func);

int gnutls_certificate_set_openpgp_key_file( gnutls_certificate_credentials_t res, 
    const char *CERTFILE, const char* KEYFILE);
int gnutls_certificate_set_openpgp_key_mem( gnutls_certificate_credentials_t res,
    const gnutls_datum_t* CERT, const gnutls_datum_t* KEY);

int gnutls_certificate_set_openpgp_keyserver(gnutls_certificate_credentials_t res,
    const char* keyserver, int port);

int gnutls_certificate_set_openpgp_trustdb(gnutls_certificate_credentials_t res,
    const char* trustdb);

int gnutls_certificate_set_openpgp_keyring_mem(
    gnutls_certificate_credentials_t c,
    const unsigned char *data, size_t dlen );

int gnutls_certificate_set_openpgp_keyring_file( gnutls_certificate_credentials_t c,
    const char *file);

int gnutls_global_init_extra(void);

/* returns libgnutls-extra version (call it with a NULL argument) 
 */
const char* gnutls_extra_check_version( const char *req_version);

#ifdef __cplusplus
}
#endif
#endif
