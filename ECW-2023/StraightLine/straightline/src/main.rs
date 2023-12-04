mod internals;

use internals::{Alice, HASH_SIZE};
use rocket::form::Form;
use rocket::FromForm;
use rocket::fs::FileServer;
use rocket::{get, launch, post, routes};
use rocket::State;
use rocket_dyn_templates::{Template, context};
use std::fs::File;
use std::io::Read;

#[derive(FromForm)]
struct UserGetToken<'r> {
    username: &'r str,
}

#[derive(FromForm)]
struct UserConnect<'r> {
    username: &'r str,
    token: String
}

#[get("/")]
fn index() -> Template {
    Template::render("index", context! { })
}

#[get("/get_token")]
fn get_token_form() -> Template {    
  Template::render("get_token", context! { } )
}

#[post("/get_token", data = "<user>")]
fn get_token_post(alice: &State<Alice>, user: Form<UserGetToken>) -> Template {
    match user.username {
        "admin" => {
          let message = "You cannot request a token for 'admin' user.".to_string();
          Template::render("error", context! { message })
        },
        _ => {
            let token = hex::encode(alice.gen_token(user.username));
            Template::render("your_token", context! { username: user.username, token})
        }
    }
}

#[get("/wonderland")]
fn wonderland_get() -> Template {
    Template::render("access", context! {})
}

#[post("/wonderland", data = "<connect>")]
fn wonderland_post(alice: &State<Alice>, flag: &State<String>, connect: Form<UserConnect>) -> Template {
    let token = match parse_token(&connect.token) {
        Some(token) => token,
        None => return Template::render("error", context! { message: "Token should be 64 hexadecimal characters exactly".to_string() })
    };
    
    if !alice.verify_token(connect.username, &token) {
        return Template::render("error", context! { message: "Wrong username or token".to_string() });
    }

    let message = match connect.username {
        "admin" => format!("Here is the flag: {}.", flag.escape_default()),
        _ => format!("Hello {}, this does not work yet, ask an admin.", connect.username)
    };
    println!("{}", message);

    Template::render("wonderland", context! { message })
}

fn parse_token(token: &String) -> Option<[u8; HASH_SIZE]> {
    let buf = match hex::decode(token) {
        Ok(buf) => buf,
        Err(_) => return None
    };
    let token: [u8; HASH_SIZE] = match buf.try_into() {
        Ok(token) => token,
        Err(_) => return None
    };
    Some(token)
}

#[launch]
fn rocket() -> _ {
    // We read the flag when execution is launched
    // and put it as global variable.
    let mut flag = String::new();
    let _n = File::open("flag.txt")
        .expect("Cannot open \"flag.txt\" file.")
        .read_to_string(&mut flag)
        .expect("Cannot read \"flag.txt\" file.");
    
    // We generate a new random key.
    let alice = Alice::init();
    rocket::build()
        .manage(alice)
        .manage(flag)
        .mount("/", routes![index, get_token_form, get_token_post, wonderland_get, wonderland_post])
        .mount("/", FileServer::from("static"))
        .attach(Template::fairing())

}