<?php


$hash = sha1('dGhlIHNhbXBsZSBub25jZQ==258EAFA5-E914-47DA-95CA-C5AB0DC85B11');
$hashdec = '';
for ($i = 0; $i < strlen($hash); $i += 2) { 
    $hashdec .= chr(hexdec(substr($hash, $i, 2))); 
};
echo base64_encode($hashdec);


?>