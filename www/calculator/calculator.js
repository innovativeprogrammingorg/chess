var pastNumber = "";
var rawInput = "";
var integer = true;
var opCount = 0;
var ans = "";
function keypress(key){
	var histonly = false
	if(pastNumber.indexOf("=")>=0){
		pastNumber = ans;
		ans = "";
		rawInput = "";
	}
	if(rawInput=="" && pastNumber=="" && key.match(/-|÷|x|\+/)!==null)
		return;
	if(key.match(/-|÷|x|\+/)!==null){
		opCount++;
		if(opCount<=1 || (opCount==2 && key=="-")){
			pastNumber += rawInput + key;
			rawInput = "";
			histonly = true;
		}
	}else{
		if(pastNumber != "" && pastNumber[pastNumber.length-1].match(/-|÷|x|\+/)===null){
			pastNumber = "";
		}
		rawInput +=key;
		opCount = 0;
	}
	if(key=="."||key=="÷"){
		integer = false;
	}
	updateScreen(histonly);
}
function updateScreen(leaveEntry){
	var main = document.getElementById("text");
	var past = document.getElementById("histtext");
	if(rawInput===""){
		if(!leaveEntry)
			main.innerHTML = "0";
		past.innerHTML = pastNumber;
	}else{
		if(!leaveEntry)
			main.innerHTML = rawInput;
		past.innerHTML = pastNumber;
	}
}
function removeAtIndex(arr,index){
	if(index>=arr.length || index<0){
		return arr;
	}
	var out = [];

	for(var i = 0;i<arr.length;i++){
		if(i==index)
			continue;
		out.push(arr[i]);
	}
	
	return out;
}
function computeAnswer(){
	var combined = pastNumber+rawInput;
	if(combined[combined.length-1].match(/-|÷|x|\+/)!==null){
		return;
	}
	opCount = 0;
	combined = combined.replace("--","+");
	var numbers = combined.match(/(!([0-9])-)?([0-9])+/g);
	var operators = combined.substring(1).match(/÷|-|\+|x/g);
	
	
	for(var i = 0;i<numbers.length;i++){
		if(numbers[i].indexOf(".")<0){
			numbers[i] = parseInt(numbers[i]);
		}else{
			numbers[i] = parseFloat(numbers[i]);
		}
	}
	var answer = solve(numbers,operators);
	if(integer||Math.round(answer)==answer){
		answer = String(Math.round(answer));
	}else{
		answer = String(answer);
		if(answer.length>answer.indexOf(".")+1)
			answer = answer.substr(0,answer.indexOf(".")+2);
	}
	pastNumber += rawInput + "="+ answer;
	rawInput = answer;
	ans = answer;
	updateScreen(false);
}
function solve(numbers,operators){
	var num = numbers;
	var op = operators;
	//alert("Multiplication");
	for(var i = 0; i<op.length;i++){
		if(op[i]=="x"){
			var result = num[i] * num[i+1];
			num[i] = result;
			op = removeAtIndex(op,i);
			num = removeAtIndex(num,i+1);
			i = 0;
			continue;
		}
	}
	
	for(i = 0; i<op.length;i++){
		if(op[i]=="÷"){
			var result = num[i] / num[i+1];
			num[i] = result;
			op = removeAtIndex(op,i);
			num = removeAtIndex(num,i+1);
			i = 0;
			continue;
		}
	}
	for(i = 0; i<op.length;i++){
		if(op[i]=="-"){
			var result = num[i] - num[i+1];
			num[i] = result;
			op = removeAtIndex(op,i);
			num = removeAtIndex(num,i+1);
			i = 0;
			continue;
		}
	}
	var out = 0;
	for(i = 0;i<num.length;i++){
		out += num[i];
	}
	return out;
}
function clearEntry(){
	rawInput = "";
	updateScreen(false);
}
function clearAll(){
	rawInput = "";
	pastNumber = "";
	integer = true;
	opCount = 0;
	updateScreen(false);
}
