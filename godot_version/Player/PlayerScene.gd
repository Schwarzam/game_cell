extends KinematicBody2D


onready var _animated_sprite = $AnimatedSprite

# Called when the node enters the scene tree for the first time.
func _ready():
	position = Vector2(0,0)
	_animated_sprite.scale = Vector2(2, 2)
	_animated_sprite.set_offset(Vector2(0, -100))
	pass # Replace with function body.

func _physics_process(delta):
	if Input.is_action_pressed("up"):
		position.y -= 1
		_animated_sprite.play("run")
	if Input.is_action_pressed("down"):
		position.y += 1
		_animated_sprite.play("run")
	if Input.is_action_pressed("right"):
		position.x += 1
		_animated_sprite.play("run")
	if Input.is_action_pressed("left"):
		position.x -= 1
		_animated_sprite.play("run")
	
	if _animated_sprite.get_animation() != "idle":
		if !(Input.is_action_pressed("up") || Input.is_action_pressed("down") ||
			Input.is_action_pressed("left") || Input.is_action_pressed("right")):
			_animated_sprite.play("idle")


