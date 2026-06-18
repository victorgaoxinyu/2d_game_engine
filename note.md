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

### Inverse of Mass
- Most physic engine store inverse of Mass instead of Mass
- most operation requires `/mass` , and `* invMass` will be cheaper

### Drag Force
- acting in the opposite direction of the relative motion with respect to surrounding fluid
- depends on velocity

```
Fdrag = 1/2 * fluid density * coefficient drag * cross-sectional area * velocity^2 * inverse of the velocity direction

```

### Friction Force
- contact force that resists sliding between surfaces
- **Kinetic friction", object is sliding along a surface, direction is opposite of movement.
- **Static friction", prevents an object from sliding, parallel to the contact surface.
```
F_friction = coefficient * F_Normal * - v_norm
```

### Gravitational Attraction Force
```
F_g = G * mass_a * mass_b / distance^2  * direction of attaction force
```
G: universal gravitational constant

### Spring Force
- Hooke's Law
  - the force of the spring is proportional to the displacement of the spring, extended or compressed. 
```
F_s = -k * delta_l
```
k: spring constant
delta_l: spring displacement

**TODO: softbody/verlet**

## Rigid body
- different than particles
- shape, rotation, and center of mass
- rigid body does not deform
- three examples
  - circles
  - polygons
  - boxes (inherit from polygon)
```cpp
enum ShapeType { BOX, POLYGON, CIRCLE };
```

### Shapes
```cpp
struct Shape {
  virtual ~Shape() = default;
  virtual ShapeType GetType() const = 0;
  virtual float GetMomentOfInertia() const = 0;
}

struct CircleShape: public Shape {
  float radius;

  CircleShape(const float radius);
  virtual ~CircleShape();
  ShapeType GetType() const override;
  float GetMomentOfInertia() const override;
}

// convec polygons for now!
struct PolygonShape: public Shape {
  vector<Vec2> vertices;

  PolygonShape() = default;
  PolygonShape(const vector<Vec2> vertices);
  virtual ~PolygonShape();
  float GetMomentofInertia() const override;
}

struct BoxShape: public PolygonShape {
  float width, height;

  BoxShape(float width, float height);
  virtual ~BoxShape();
  float GetMomentofInertia() const override;
}

```

### Angular velocity and Angular Acceleration
- angle: *theta*, radian
- angular velocity: *omega*
- angular acceleration: *alpha*


How to initiate rotational motion? -> Torque

### Torque
- torque: *tau*
```
F_tau = F * d * sin(beta)
```
F_tau: Torque force
F: 
d: distance from the center to the point where the force is being applied
beta: 
```
Tau = I * alpha
```
I: Moment of inertia, is analogous to mass, measures how much object resists to change its angular acceleration.
- moment of inertia is **inversely proportional** to angular acceleration

different moment of inertia for different shapes
```
# solid circle
I = r^2 / 2 * m

# solid rectangle
I = (w^2 + h^2) / 12 * m 
```

### Box Vertices
Local space -> World space

### OOP
why not implement the draw method in Shape classes?
- because we dont want to have render-related code in **Physics** engine.

## Circle-circle collision detection
- Collision Detection
- Collision Resolution

## Collision Contact Information
- Start and end contact points (from "a" to "b")
- Normal direction is always from "a" to "b"
- Depth is the penetration length of the collision

```cpp
struct Contact {
  Body* a;
  Body* b;

  Vec2 start;
  Vec2 end;

  Vec2 normal;
  float depth;
}
``` 

### Collision system
Collision detection -> Compute collision information -> Collision Resolution
- detection is shape-specific
- resolution is shape agnostic


### Broad Phase and Narrow Phase
- Broad Phase
  - check bodies that *probably* gonna collide.
  - use a faster algo, like bounding circles or bounding boxes
- Narrow Phase
  - check if objects are *indeed* colliding.

### Collision resolution
- assume collision detection provides
  - Collision normal
  - contact points for both bodies
  - Penetration depth

- **Collision normal** is the direction in which the potential impulse should be applied
- **Penetration depth** (along with some other things) will determine how big of the impulse


Methods
- Projection method
```
# using mass
d_a = depth * m_b / (m_a + m_b)
d_b = depth * m_a / (m_a + m_b)

# using invMass

d_a = depth / (1/m_a + 1/m_b) * 1/m_a
d_b = depth / (1/m_a + 1/m_b) * 1/m_b
```
```cpp
void Contract::ResolvePenetration() {
  float da = depth / (a->invMass + b->invMass) * a->invMass;
  float db = depth / (a->invMass + b->invMass) * b->invMass;
}

```