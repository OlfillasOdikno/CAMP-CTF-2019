<?php
require_once("config.php");
session_start();

function evaluate($p){
	$descriptorspec = array(
	   0 => array("pipe", "r"),
	   1 => array("pipe", "w")
	);
	$process = proc_open("python3 pyMalbolge.py", $descriptorspec, $pipes);

	if (!is_resource($process)) {
		panic();
	}

	fwrite($pipes[0], $p);
	fclose($pipes[0]);
	$page = stream_get_contents($pipes[1]);
	fclose($pipes[1]);
	proc_close($process);
	if(empty($page)){
		panic();
	}
	return $page;
}

function panic(){
	setcookie("p", $m_panic,  time() + (86400 * 30),"/");
	require_once("public/panic.html");
	exit();
}

function panic_handler($errno, $errstr, $errfile, $errline) {
	panic();
}

set_error_handler("panic_handler");

if(!isset($_SESSION['user'])) {
	if(isset($_COOKIE['p'])){
		$p = evaluate(base64_decode($_COOKIE['p']));
		if($p === "register.php" || $p === "login.php" ){
			require_once($p);
			exit();
		}
	}
	#login.php
    setcookie("p", $m_login,  time() + (86400 * 30),"/");
    header("Refresh:0");
    exit();
}

if (!isset($_COOKIE['p'])) {
	#public/panic.html
	setcookie("p", $m_panic,  time() + (86400 * 30),"/");
	header("Refresh:0");
	exit();
}

$location=realpath(evaluate(base64_decode($_COOKIE['p'])));
if(substr($location, 0, strlen(__DIR__ . DIRECTORY_SEPARATOR)) === __DIR__ . DIRECTORY_SEPARATOR){
	require_once($location);
}else{
	panic();
}