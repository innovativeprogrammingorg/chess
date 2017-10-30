<?php

require_once "sql.php";

parse_str(implode('&', array_slice($argv, 1)), $_POST);

$gid = intval($_POST['gid']);
$user = $_POST['user'];
$conn = SQLConn::getConn();
$stmt = $conn->prepare("SELECT * FROM chessGame WHERE ID = ?");
$stmt->bind_param('i',$gid);
$game = SQLConn::getResult($stmt);
if($game == NULL){
	exit("NOT FOUND");
}
$sep = chr(31);
$side = 'b';
if($game['White'] == $user){
	$side = 'w';
}

$out = $side . $sep . $user . $sep . $gid . $sep . $game['Board'] . $sep . $game['Turn'] . $sep . $game['I'] . $sep . $game['WTime'] . $sep . $game['BTime']; 
exit($out);

?>