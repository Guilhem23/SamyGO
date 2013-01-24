<?php
function salted_key_and_iv($td, $passphrase, $salt){
	if($td === false){
		$key_len = openssl_cipher_iv_length('AES-128-CBC');
		$iv_len = openssl_cipher_iv_length('AES-128-CBC');
	} else {
		$key_len = mcrypt_enc_get_key_size($td);
		$iv_len  = mcrypt_enc_get_iv_size($td);
	}
	$desired_len = $key_len + $iv_len;
	$data = $dx = ""; /* orig define */
	while (strlen($data) < $desired_len) {
		$dx = HexToStr(md5($dx . $passphrase . $salt));
		$data .= $dx;
	}
	return array(substr($data, 0, $key_len), substr($data, $key_len, $iv_len));
}

function n_encrypt_ssl($cleartext, $passphrase, $salt = false, $method = "aes-128-cbc", $raw_input = true){
	$salt = ($salt === false) ? "SamyGOwn" : $salt;
	list($key, $iv) = salted_key_and_iv(false, $passphrase, $salt);
	$ciphertext = 'Salted__' . $salt . openssl_encrypt($cleartext, $method, $key, $raw_input, $iv);
	CError::LogStr("Crypto Engine Request (".mt_rand()."): -> $cleartext");
	while ($msg = openssl_error_string())
		CError::LogStr("Crypto Engine: clean messages -> $msg");
	return $ciphertext;
}
function n_decrypt_ssl($ciphertext, $passphrase, $method = "aes-128-cbc", $raw_output = true){
	$salt = substr($ciphertext, 8, 8);
	list($key, $iv) = salted_key_and_iv(false, $passphrase, $salt);
	$cleartext = openssl_decrypt(substr($ciphertext, 16), $method, $key, $raw_output, $iv);
	while ($msg = openssl_error_string())
		CError::LogStr("Decrypt Engine: clean messages -> $msg");
	return $cleartext;
}
function GetDataSig($data, $rsaprivkeyfile = 'file:///home/Video-HD/Samsung/mykey.pem')
{
	$rsakey = openssl_pkey_get_private($rsaprivkeyfile);
	if (!$rsakey) return(false);

	$sig = false;
	if (!@openssl_sign($data, $sig, array($rsakey, ''))) return(false);
	$sigHex = strToHex($sig);
	return($sigHex);
}
function file_hash_otn($algo = "crc32", $filename){
	$hash = hash_file($algo, $filename);
	$array = unpack('N', pack('H*', $hash));
	$u_hash = sprintf("%u", $array[1]);
	return $u_hash;
}
function file_hash_ul($algo, $filename){
	$hash = hash_file($algo, $filename);
	$array = unpack('I', pack('H*', $hash));
	$u_hash = sprintf("%u", $array[1]);
	return $u_hash;
}
?>
