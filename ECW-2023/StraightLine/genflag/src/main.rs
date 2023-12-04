use rand::{
    Rng, thread_rng,
    distributions::Alphanumeric
};
use std::fs::File;
use std::io::Write;

fn genflag() -> String {
    let random_part_of_flag: String = thread_rng().sample_iter(&Alphanumeric).take(24).map(char::from).collect();

    format!("ECW{{straight_line_{}}}", random_part_of_flag)
}

fn main() {
    let flag = genflag();
    let mut file = File::create("flag.txt")
        .expect("Cannot create file \"flag.txt\".");
    file.write_all(flag.as_bytes())
        .expect("Cannot write flag in file \"flag.txt\".");
}
