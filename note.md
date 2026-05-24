### Outline
- Mechanics
- Rigid Bodies
- collision Detection
- Collision Resolution


2D Vectors
```cpp
struct Vec2 {
    float x;
    float y;

    // constructor
    Vec2(float x, float y): x(x), y(y) {}
    
    // destructor
    ~Vec2() = default;
}

int main() {
    Vec2 a(3.7, 25.0);
    Vec2 b(4.5, -2.0);
}
```

Most game engine use top left cornor as (0, 0)

#### Practice
```js
// DVD idle  
var x = 10;
var y = 10;
var d = 10;
var dx = 1;
var dy = 1;

function setup() {
  createCanvas(400, 500);
}

function draw() {
  background("black");

  if (x == 400 || x == 0 ) {
    dx *= -1
  }
  if (y == 500 || y == 0) {
    dy *= -1
  }

  x += dx * 2;
  y += dy * 2;
  // d += 1;
  circle(x, y, d)
}
```

### Vector Operations
Magnitude
```
||v|| = sqrt((x^2) + (y^2))
```

### Vector Addition & Subtraction
```
a + b = (ax + bx, ay + by)
a - b = (ax - bx, ay - by)
```

```js
class Vec2 {
  constructor(x, y) {
    this.x = x;
    this.y = y;
  }
  add(v) {
    this.x += v.x;
    this.y += v.y;
  }
  sub(v) {
    this.x -= v.x;
    this.y -= v.y;
  }
  mag() {
    return Math.sqrt(this.x^2 + this.y^2);
  }
  draw(color) {
    fill(color);
//     stroke("white");
    // line(0, 0, this.x, this.y);
    circle(this.x, this.y, 10);
  }
}

let position = new Vec2(10, 20);
let velocity = new Vec2(2, 1);

function setup() {
  createCanvas(400, 500);
}

function draw() {
  background("black");
  position.add(velocity)
  // position.add(velocity)
  
  position.draw("red");
  
}
```

### Static Method
```js
static add (a, b) {
  let result = new Vec2(0, 0);
  result.x = a.x + b.x;
  result.y = a.y + b.y;
  return result;
}
```
## Dot product and Cross product
### Dot product
- Doc product == scalar product
- `a dot b`, means the vec b projection on vec a
- result is a scalar
- cummutative, `a dot b == b dot a`

```
a dot b = ax * bx + ay * by
```

### Cross Product
- normal vector that perpendicular to the plan constructed by vec a and vec b
```
nx = ay * bz - az * by
ny = az * bx - ax * bz
nz = ax * by - ay * bx
```
- Not cummutative `a cross b != b cross a`
1, 6, -8
4, -2, -1

-22, 
-32 + 1

## Vector Normalisation
- when we only care direction
```
a_norm = a / a_mag
```

## Vector scaling, translation, and rotating
```
x' = x * cos(beta) - y * sin(beta)
y' = x * sin(beta) + y * cos(beta)
```

## C++ part
Operator overloading
```
v1.Add(v2)   ->  v1 = v1 + v2
v1.Sub(v2)   ->  v1 = v1 - v2
v1.Scale(n)  ->  v1 = v1 * n
v1.Scale(-1) ->  v1 = v1 * -1
```


## Partical Physics
- partical objects <-> rigid bodies
- no shape, simple `point-mass` objects
- newtonian mechanics, three laws of motion
  - inertia: if no force is applied on an object, its velocity shall not change
  - force, mass, acceleration: `F=ma`
  - action and reaction

```cpp
struct Particle {
  Vec2 position;
  Vec2 velocity;
  Vec2 acceleration;

  float mass;

  Particle(float x, float y, float mass);
  ~Particle();
}
```