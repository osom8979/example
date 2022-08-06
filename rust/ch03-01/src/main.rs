// https://rinthel.github.io/rust-lang-book-ko/ch03-01-variables-and-mutability.html

fn main0() {
    let x = 5;
    println!("The value of x is: {}", x);
    // x = 6;  // Error !
    println!("The value of x is: {}", x);
}

fn main1() {
    let mut x = 5;
    println!("The value of x is: {}", x);
    x = 6;
    println!("The value of x is: {}", x);
}

const MAX_POINT: u32 = 100_000;

fn main2() {
    let x = MAX_POINT;
    let x = x + 1;

    // prev `x` Shadowing ...

    let x = x + 2;

    println!("The value of x is: {}", x);
}

fn main3() {
    let space = "  ";  // shadowing
    let space = space.len();
    println!("space len is: {}", space);
}

fn main() {
    // main0();
    // main1();
    // main2();
    main3();
}

