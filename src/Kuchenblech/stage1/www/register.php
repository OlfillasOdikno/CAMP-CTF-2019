<?php
require_once("config.php");

if($_SERVER["REQUEST_METHOD"] == "POST") {
    header("Content-type: application/json");
    
    // parse JSON from post data
    $data = json_decode(file_get_contents('php://input'));
    if($data === null){
        $response = array(
            'status' => 0,
            'msg' => "Invalid JSON request!"
        );
        echo json_encode($response);
        exit();
    }

    $query = "INSERT INTO users (username, pass) VALUES (?, ?)";
    if($stmt = mysqli_prepare($db, $query)){
        $stmt->bind_param("ss", $username, $password);
        
        $username = "";
        $password = "";
        if(isset($data->username) && isset($data->password)){
            $username = $data->username; 
            $password = md5($data->password);
        
            $result = false;
            $result = $stmt->execute();
            if($result === true) {
                $response = array(
                    'status' => 1,
                    'msg' => "Registration successful!"
                );
            
            }  else {
                $response = array(
                    'status' => 0,
                    'msg' => "Invalid username!"
                );        
            }

            $stmt->close();
        }

    } else {
        $response = array(
            'status' => 0,
            'msg' => "DB query failed!"
        );     
    }
    echo json_encode($response);
}else{
    include ('./public/register.html');
}
