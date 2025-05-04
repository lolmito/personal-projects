function Calculator(){
    this.operations = {
        '+': (a,b) => a + b,
        '-': (a,b) => a - b,
    }
    this.calculate = function(string) {
        let parts = string.split(/ *([+\-*/]) */) //["3", "+", "7"]
        let number1 = +parts[0];
        let operator = parts[1];
        let number2 = +parts[2];
        if(!this.operations[operator]) return NaN;
        return this.operations[operator](number1,number2);
    }
    this.addMethod = function (op,func){
        this.operations[op] = func;
        return "Added function: " + op;
    }
}