<?php

class SQLConn{
	private static $host = 'localhost';

	private static $admin = 'root';//'admin172';
	private static $admin_pass = 'password'; //'hB1lT^F+,O9T';
	private static $database = 'chessClub';

	public static function getConn(){
		return new mysqli(self::$host, self::$admin, self::$admin_pass, self::$data_database);
	}
	/**
	 * Always returns results in a 2D array 
	 * @param  mysqli_stmt $stmt The prepared statement
	 * @return array       An array of the results or null
	 */
	public static function fetchResults($stmt){
		$stmt->execute();
		$results =  self::fetch($stmt);
		if(count($results)==0){
			return array();
		}
		return $results;
	}

	public static function getResults($stmt){
		$stmt->execute();
		$results =  self::fetch($stmt);
		if(count($results)==0){
			return NULL;
		}
		return $results;
	}

	public static function getResult($stmt){
		$results = self::getResults($stmt);
		if(count($results)==1){
			return $results[0];
		}
		return $results;
	}
	
	public static function fetch($result){    
    	$array = array();
    	assert($result instanceof mysqli_stmt);
    
        $result->store_result();
        $variables = array();
        $data = array();
        $meta = $result->result_metadata();
        
        while($field = $meta->fetch_field()){
            $variables[] = &$data[$field->name]; 
        }
        
        call_user_func_array(array($result, 'bind_result'), $variables);
        
        $i=0;
        while($result->fetch()){
            $array[$i] = array();
            foreach($data as $k=>$v)
                $array[$i][$k] = $v;
            $i++;
        }
    	return $array;
	}
}


?>