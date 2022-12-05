extends Node2D

onready var _animated_sprite = $AnimatedSprite

# Called when the node enters the scene tree for the first time.
func _ready():
	scale = Vector2(2, 2)
	_animated_sprite.play("idle")
	
	position = Vector2(100, 100)


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
