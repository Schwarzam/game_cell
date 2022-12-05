extends KinematicBody2D


onready var _animated_sprite = $AnimatedSprite

var attacking = false
var SPEED = 100
var motion = Vector2(0, 0)
# Called when the node enters the scene tree for the first time.
func _ready():
	_animated_sprite.connect("animation_finished", self, "_on_attack_finished")

func _on_attack_finished():
	self.attacking = false
	

func run_action(action):
	if not self.attacking:
		_animated_sprite.play(action)
		
		
func _process(delta):
	if self.attacking:
		if _animated_sprite.frame > 7 and _animated_sprite.frame < 11:
			get_node("Area2D/CollisionShape2D2").disabled = false
		else:
			get_node("Area2D/CollisionShape2D2").disabled = true
			
		if _animated_sprite.frame > 10 and _animated_sprite.frame < 14:
			get_node("Area2D/CollisionShape2D").disabled = false
		else:
			get_node("Area2D/CollisionShape2D").disabled = true

func _physics_process(delta):
	if Input.is_action_pressed("up"):
		motion.y -= SPEED
		run_action("run")
	if Input.is_action_pressed("down"):
		motion.y += SPEED
		run_action("run")
	if Input.is_action_pressed("right"):
		motion.x += SPEED
		run_action("run")
	if Input.is_action_pressed("left"):
		motion.x -= SPEED
		run_action("run")
		
	move_and_slide(motion, Vector2(0, 0), false, 4, 0.785, true)
	
	if _animated_sprite.get_animation() != "idle":
		if !(Input.is_action_pressed("up") || Input.is_action_pressed("down") || Input.is_action_pressed("left") || Input.is_action_pressed("right")):
			run_action("idle")
		motion = Vector2(0, 0)
		
	for index in get_slide_count():
		var collision = get_slide_collision(index)
		#print(collision)

func _unhandled_input(event):
	if event is InputEventMouseButton:
		self.attacking = true
		_animated_sprite.play("attack")
		
		EntitiesControler.print_tei()
		


func _on_Area2D_body_entered(body):
	print(body)
