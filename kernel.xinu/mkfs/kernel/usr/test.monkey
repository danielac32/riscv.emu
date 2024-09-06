// Variable declarations
let a = 5;
let b = 25;

// Reassign (previously declared) variables
a = a * 5;

// For loops
for (let i = 0; i < 10; i = i + 1) {
    // i = 0 .. 9
}

// While loops
while (b > a) {
    b = b - 5;
    break;
}

// If statements
if (b == a || a == b) {
    puts("b equals a!");
}

// Strings
let c = "Hello world";
c = str_split(c, " "); 
type(c); // "ARRAY"
len(c); // 2 

// Arrays
let d = [5, true, "Monkey"];
d[0]; // 5
array_push(d, 10);
array_pop(d); // 10

// Functions
let fibonacci = fn(x) {
    if (x < 2) {
        return x;
    }

    return fibonacci(x - 1) + fibonacci(x - 2);
}

let square = fn(x) {
    return x * x;
}


puts("square");
let aw = 10;
let bw = 20;
let result = square(aw) + bw * bw - 100 / 5 % 8;
puts(result);

puts("euler");
let sum = 0;
for (let i=1; i < 1000; i=i+1) {
    if (i % 3 == 0 || i % 5 == 0) {
        sum = sum + i;
    }
}
puts(sum);
let str = "Comma,separated,values,daniel,quintero";
let arr = str_split(str, ",");
for (let i=0; i < len(arr); i=i+1) {
    puts(i, ": ", arr[i]);
}
// Built-in functions
puts("10th fibonacci number is: ", fibonacci(20));
puts("Type of c = ", type(c));
puts("Length of c = ", len(c));
puts("Integer value of true = ", int(true));
