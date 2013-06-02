#
# Configuration file for using the XML library in GNOME applications
#
prefix="/usr"
exec_prefix="/usr"
libdir="/usr/lib"
includedir="/usr/include"

XMLSEC_LIBDIR="/usr/lib"
XMLSEC_INCLUDEDIR=" -D__XMLSEC_FUNCTION__=__FUNCTION__ -DXMLSEC_NO_XSLT=1 -DXMLSEC_NO_XKMS=1 -I/usr/include/xmlsec1   -I/opt/montavista/foundation/devkit/arm/v5t_le/target/usr/include/libxml2      -DXMLSEC_OPENSSL_098=1 -DXMLSEC_CRYPTO_OPENSSL=1 -DXMLSEC_CRYPTO=\\\"openssl\\\""
XMLSEC_LIBS="-L/usr/lib -lxmlsec1-openssl -lxmlsec1  -ldl  -lxml2    -lssl -lcrypto  "
MODULE_VERSION="xmlsec-1.2.9-openssl"

