extends KinematicBody2D


onready var _animated_sprite = $AnimatedSprite

var attacking = false

# Called when the node enters the scene tree for the first time.
func _ready():
	_animated_sprite.connect("animation_finished", self, "_on_attack_finished")

func _on_attack_finished():
	self.attacking = false
	

func run_action(action):
	if not self.attacking:
		_animated_sprite.play(action)
		

func _physics_process(delta):
	if Input.is_action_pressed("up"):
		position.y -= 1
		run_action("run")
	if Input.is_action_pressed("down"):
		position.y += 1
		run_action("run")
	if Input.is_action_pressed("right"):
		position.x += 1
		run_action("run")
	if Input.is_action_pressed("left"):
		position.x -= 1
		run_action("run")
	if _animated_sprite.get_animation() != "idle":
		if !(Input.is_action_pressed("up") || Input.is_action_pressed("down") || Input.is_action_pressed("left") || Input.is_action_pressed("right")):
			run_action("idle")

func _unhandled_input(event):
	if event is InputEventMouseButton:
		self.attacking = true
		_animated_sprite.play("attack")
		
