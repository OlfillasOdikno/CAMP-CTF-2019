<?php

$uploadDir = "./public/static/img/profile/";
$uploadFile = $uploadDir . sha1(session_id()) . ".png";

if($_SERVER['REQUEST_METHOD'] == "POST") {
    header("Content-type: application/json");

    $image = file_get_contents('php://input');
    if($image === false){
        $response = array(
            'status' => 0,
            'msg' => "Data read failure!"
        );
        echo json_encode($response);
        exit();
    }

    // Do some silly image validataion
    if( $_SERVER['CONTENT_TYPE'] !== 'image/png' || \
        strcmp(substr($image, 0, 8), "\x89PNG\x0d\x0a\x1a\x0a") !== 0) {
        
            $response = array(
                'status' => 0,
                'msg' => "Invalid image!"
            );
    } else {
        // Image is valid :P
        if(file_put_contents($uploadFile, $image)){
            $response = array(
                'status' => 1,
                'msg' => "Upload successful!"
            );
        } else {
            $response = array(
                'status' => 0,
                'msg' => "Upload failed!"
            );
        }
    }
    
    echo json_encode($response);
}