<?php
require_once 'firebaseLib.php';
// --- Aqui se asigna la URL de la base de datos en Firebase
$url = 'https://senmin-cb7cc-default-rtdb.firebaseio.com/';
// --- Consume el Token de la base de datos en la siguiente linea
$token = '7HElDOyIX5VNIGrlCwBthEkHD4W2XD16VP2EUXtm';
// --- Desde aquí obtiene el parametro que se enviará por http GET
$arduino_data = $_GET['acidoSulfValor'];
// --- $arduino_data_post = $_POST['name'];
// --- Aquí asigna la parte de la estrucutra de la base de datos donde irá el parámetro
$firebasePath = '/Dispositivo/0/acidoSulf';
/// --- Generar llamadas
$fb = new fireBase($url, $token);
$response = $fb->set($firebasePath, $arduino_data);
sleep(2);
