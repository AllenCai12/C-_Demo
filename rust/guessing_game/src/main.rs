use std::io;
use rand::Rng;

fn main() {

    let screte_number = rand::thread_rng().gen_range(1, 101);
    println!("You get screte number : {}", screte_number );

    println!("Please input your guess number: ");
    let mut guess = String::new();
    io::stdin().read_line(&mut guess).expect("Fail to read line");

    println!("You guessed: {}", guess);

}
