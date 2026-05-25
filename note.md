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

## Frame Rate
waste some time if not yet reach target `MILLISECS_PER_FRAME`
```cpp
const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;
//...

int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
if (timeToWait > 0) {
  SDL_Delay(timeToWait);
}

timePreviousFrame = SDL_GetTicks();
// ...
```

Delta Time: difference between the current frame and last frame (in seconds)

```cpp
float deltaTime = (SDL_GetTicks() - timePerviousFrame) / 1000.0f;

particle->velocity.x = 50.0 * deltaTime;
...

// if I want to move 50 px per second on x and 10 px per sec on y
// not move how many px per frame
```

instead of thinking how many px we want to move **per frame**, we need to think how many px to move **per second**, this is because we want to have **frame rate independent movement**

## Simulation movement
- physics software simulation works by making several small predictions
- using mathematicall technique called **integration**
- the smaller the delta-time, the better will be our prediction

`v = dp/dt`, the derivative of position is velocity

`a = dv/dt`, the derivative of velocity is acceleration

### Numerical integration
- Euler integration is the most basic technique
  - accuracy is "good enough" for most game apps
  - drawback is requires solving a system of equations per time-step
- Verlet integration 
  - better accuracy, less memory usage when simulating large number of particles
- Runge-kutta integration
  - a family of integrators
  -  RK4 is the most popular implementation
  -  accumulates error is on the order of the 4th derivative, very accurate`

## Force
`F = ma`

```cpp
// -> Apply a wind force to the right
Vec2 wind = Vec2(2.0, 0.0);
particle->AddForce(wind);
...
particle->Integrate(deltaTime);
```

```cpp
void Particle::AddForce(const Vec2& force) {
  sumForces += force;
}

void Particle::Integrate(float dt) {
  acceleration = sumForces / mass;
  velocity += acceleration * dt;
  position += velocity * dt;

  ClearForces();  // clear the sumForces for next frame/step
}
```