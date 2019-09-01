<?php
define('DB_SERVER', 'db');
define('DB_USERNAME', 'root');
define('DB_PASSWORD', 'y0ctGLRXkLTI7am');
define('DB_DATABASE', 'db');

$m_login = "KCcmJDpeIiF9fDRYenl3NS5SdHNycSgnS21saighRWdmZSJ5P2E8PE06OTg3WTVXVVRTUi9RbGVkY2JLJ0lIR0ZufjJBe1xoPT07dTp0VHI2SzRuIWxrWFdoQmZlZGNicyRNcD5+W0hZV1ZEVWZlUVFyKk5M";

$m_panic = "RCdgTl5Mb348fXtYeXhUNTQzLGJPYG9MLCtISGo0aERDQUEuYVBgXylceHdwdW5zcmsxb25nT2tkKktnYF9eJEVhYF9eXVZ6VFlSdlA4Tk1McDNJTk1GakpJSEEpP2MmQkFAPz49PDVZenk3MDU0MzIrMC8oTG0lSSkoaCZ9fCMieT9gfHV6eXh3cDZ0c2xUajBuUE9lK0xLZ2ZlXiRFYWBfXl1Wemc=";

$m_chat = "KCclJTpeIn59fXt6ejFVd3V0dCswKShMbm1rKSJGaGckIyJ5Pz1PXyk6eFtZb3RzbDJTb2guT2UrdmhnOV9HJDUiIV9eV0A=";

$db = mysqli_connect(DB_SERVER,DB_USERNAME,DB_PASSWORD,DB_DATABASE);
if($db === false) {
    die("DB connection failed!Error: " . mysqli_connect_error());
}
