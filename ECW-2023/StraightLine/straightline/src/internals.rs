use rand_core::{OsRng, RngCore};

const BLOCK_SIZE: usize = 136;
const KEY_SIZE: usize = 16;
pub const HASH_SIZE: usize = 32;
pub type Hash = [u8; HASH_SIZE];
pub type Key = [u8; KEY_SIZE];

pub struct Alice {
    key: Key
}

impl Alice {
    pub fn init() -> Alice {
        let mut key: Key = [0u8; KEY_SIZE];
        OsRng.fill_bytes(&mut key);
        Alice { key }
    }

    pub fn gen_token(&self, username: &str) -> Hash {
        // hash(key || message)
        let mut payload = vec![0u8; self.key.len() + username.as_bytes().len()];
        payload[..self.key.len()].copy_from_slice(&self.key);
        payload[self.key.len()..].copy_from_slice(username.as_bytes());
        alice_hash(&payload)
    }

    pub fn verify_token(&self, username: &str, token: &Hash) -> bool {
        let token_calc = self.gen_token(username);
        token_calc == *token
    }
}

fn alice_hash(msg: &[u8]) -> Hash {
    let mut state = State::new();
    
    let iter = msg.chunks_exact(BLOCK_SIZE);
    let rem = iter.remainder();
    for chunk in iter {
        state.xor(chunk.try_into().unwrap());
        state.f();
    }

    let mut last_block = [0u8; BLOCK_SIZE];
    last_block[..rem.len()].copy_from_slice(rem);
    pad(&mut last_block, rem.len());
    state.xor(&last_block);
    state.f();

    state.digest()
}

#[derive(Clone, Copy)]
struct State {
    inner: [u64; 25]
}

impl std::ops::Index<(usize, usize)> for State {
    type Output = u64;

    fn index(&self, idx: (usize, usize)) -> &Self::Output {
        &self.inner[(idx.0 % 5) + 5*(idx.1 % 5)]
    }
}

impl std::ops::IndexMut<(usize, usize)> for State {
    fn index_mut(&mut self, idx: (usize, usize)) -> &mut Self::Output {
        &mut self.inner[(idx.0 % 5) + 5*(idx.1 % 5)]
    }
}

const RC: [u64; 24] = [
    0x0000000000000001, 0x0000000000008082, 0x800000000000808A, 0x8000000080008000,
    0x000000000000808B, 0x0000000080000001, 0x8000000080008081, 0x8000000000008009,
    0x000000000000008A, 0x0000000000000088, 0x0000000080008009, 0x000000008000000A,
    0x000000008000808B, 0x800000000000008B, 0x8000000000008089, 0x8000000000008003,
    0x8000000000008002, 0x8000000000000080, 0x000000000000800A, 0x800000008000000A,
    0x8000000080008081, 0x8000000000008080, 0x0000000080000001, 0x8000000080008008
];

impl State {

    fn new() -> State {
        State { inner: [0; 25] }
    }
    
    fn iota(&mut self, ir: usize) {
        self[(0, 0)] ^= RC[ir];
    }

    fn rho(&mut self) {
        let mut xx = 1;
        let mut yy = 0;
        for t in 0..24 {
            self[(xx, yy)] = self[(xx, yy)].rotate_left((t + 1)*(t + 2)/2);
            let tmp = xx;
            xx = yy;
            yy = (2*tmp + 3*yy) % 5;
        }
    }

    fn pi(&mut self) {
        let a = *self;
        for x in 0..5 {
            for y in 0..5 {
                self[(x, y)] = a[(x + 3*y, x)];
            }
        }
    }

    fn theta(&mut self) {
        let mut c = [0u64; 5];
        for x in 0..5 {
            c[x] = self[(x, 0)];
            for y in 1..5 {
                c[x] ^= self[(x, y)];
            }
        }

        let mut d = [0u64; 5];
        for x in 0..5 {
            let mut temp = c[(x + 1) % 5];
            temp = temp.rotate_left(1);
            d[x] = temp ^ c[((5 + x) - 1) % 5];
        }
        for x in 0..5 {
            for y in 0..5 {
                self[(x, y)] ^= d[x];
            }
        }
    }

    fn f(&mut self) {
        for ir in 0..24 {
            self.theta();
            self.rho();
            self.pi();
            self.iota(ir);
        }
    }

    fn as_bytes(&self) -> &[u8; 200] {
        unsafe { std::mem::transmute(self) }
    }

    fn as_bytes_mut(&mut self) -> &mut [u8; 200] {
        unsafe { std::mem::transmute(self) }
    }

    fn xor(&mut self, a: &[u8; BLOCK_SIZE]) {
        let state_bytes = self.as_bytes_mut();
        state_bytes[..BLOCK_SIZE].copy_from_slice(a);
    }

    fn digest(&self) -> Hash {
        let mut hash = [0u8; HASH_SIZE];
        hash.copy_from_slice(&self.as_bytes()[..HASH_SIZE]);
        hash
    }
}

fn pad(block: &mut [u8; BLOCK_SIZE], len: usize) {
    assert!(len < block.len());
    block[len] = 0x06;
    block[block.len() - 1] |= 0x80;
}
