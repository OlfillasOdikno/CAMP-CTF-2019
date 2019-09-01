<?php

// Used by the chat :P
setcookie("nick", $_SESSION['user'], time() + (86400 * 30), "/"); // 1 day

require_once("./public/index.html");