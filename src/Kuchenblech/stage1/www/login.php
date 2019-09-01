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

    $query = "SELECT username, pass FROM users WHERE username = ? AND pass = ?";
    if($stmt = mysqli_prepare($db, $query)){
        $stmt->bind_param("ss", $username, $password);

        $username = "";
        $password = "";
        if(isset($data->username) && isset($data->password)){
            $username = $data->username; 
            $password = md5($data->password);

            $result = false;
            $result = $stmt->execute();
            if($result === true){
                
                $stmt->store_result();
                if($stmt->num_rows === 1){
                    $response = array(
                        'status' => 1,
                        'msg' => "Login successful!"
                    );

                    $_SESSION['user'] = $username;
                    #chat.php
                    setcookie("p", $m_chat,  time() + (86400 * 30),"/");
                    header("Refresh:0");
                    echo json_encode($response);
                    exit();
                } else {
                    $response = array(
                        'status' => 0,
                        'msg' => "Login Failed!"
                    );
                }
            } else {
                $response = array(
                    'status' => 0,
                    'msg' => "Execute failed!"
                );
            }
        } else {
            $response = array(
                'status' => 0,
                'msg' => "username or password missing!"
            );  
        }

        $stmt->close();

    } else {
        $response = array(
            'status' => 0,
            'msg' => "DB query failed!"
        );    
    }
    echo json_encode($response);
}else{
    include ('./public/login.html');
}