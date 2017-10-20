<?php

	parse_str(implode('&', array_slice($argv, 1)), $_POST);
	$first_name = $_POST['first_name'];
	$last_name = $_POST['last_name'];


	echo "<script>
			alert(\"Hello ${first_name} ${last_name}\");
			</script>";


	//phpinfo();

