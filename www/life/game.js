var grid = [];
var width = 100;
var height = 100;
var generations = 0;
var run = undefined;
var timeGap = 100;

function init(){
	generations = 0;
	var out = "";
	for(var i = 0;i<height;i++){
		out +="<tr>";
		grid.push(new Array());
		for(var j = 0;j<width;j++){
			grid[i][j] = Math.random()<0.2;
			out += "<td onclick='switchCell("+i+","+j+")'></td>";
		}
		out +="</tr>";
	}
	document.getElementsByTagName("table")[0].innerHTML = out;
	display();
	startGame();
}
function startGame(){
	var buttons = document.getElementsByTagName("button");
	buttons[0].style.backgroundColor = "#666";
	buttons[1].style.backgroundColor = "#222";
	run = setInterval(nextGen,timeGap);
}
function pauseGame(){
	var buttons = document.getElementsByTagName("button");
	buttons[0].style.backgroundColor = "#222";
	buttons[1].style.backgroundColor = "#666";
	clearTimeout(run);
}
function clearGame(){
	for(var i=0;i<height;i++){
		for(var j = 0;j<width;j++){
			grid[i][j] = false;
		}
	}
	generations = 0;
	display();
}
function startLocation(event){
	XPOSi = event.clientX;
    YPOSi = event.clientY;
}
function endLocation(event){
	XPOSf = event.clientX;
    YPOSf = event.clientY;
    document.getElementsByTagName("body")[0] += XPOSi +" " + YPOSi + " " + XPOSf + " " + YPOSf + "<br>";
}
function nextGen(){
	var newGrid = [];
	for(var i = 0;i<height;i++){
		newGrid.push(new Array());
		for(var j = 0;j<width;j++){
			var count = 0;
			if(grid[(i>0)? i-1 : height-1][(j>0)? j-1 : width-1]) 
				count++;
			if(grid[(i>0)? i-1 : height-1][j]) 
				count++;
			if(grid[(i>0)? i-1 : height-1][(j+1)%width]) 
				count++;
			if(grid[i][(j>0)? j-1 : width-1]) 
				count++;
			if(grid[i][(j+1)%width]) 
				count++;
			if(grid[(i+1)%height][(j>0)? j-1 : width-1]) 
				count++;
			if(grid[(i+1)%height][j]) 
				count++;
			if(grid[(i+1)%height][(j+1)%width]) 
				count++;
			if(count<2){
				newGrid[i][j]=false;
			}else if(count == 2 && grid[i][j]){
				newGrid[i][j]=true;
			}else if(count == 3){
				newGrid[i][j]=true;
			}else{
				newGrid[i][j]=false;
			}
		}
	}
	grid = newGrid;
	generations++;
	display();
}
function display(){
	var cells = document.getElementsByTagName("td");
	for(var i = 0;i<height;i++){
		for(var j = 0;j<width;j++){
			if(grid[i][j]){
				cells[j+(height *i)].style.backgroundColor = "#F00";
			}else{
				cells[j+(height *i)].style.backgroundColor = "#000";
			}
			
		}
	}
	document.getElementsByTagName("span")[0].innerHTML = generations;
}
function switchCell(x,y){
	grid[x][y] = !grid[x][y];
	display();
}
