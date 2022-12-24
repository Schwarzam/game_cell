extends Node2D

onready var _animated_sprite = $AnimatedSprite
var lock_animation = false
# Called when the node enters the scene tree for the first time.
func _ready():
	scale = Vector2(2, 2)
	_animated_sprite.play("idle")
	position = Vector2(rand_range(0, 300), rand_range(0, 300))
	
	EntitiesControler._register_enitity(get_instance_id())
	
func _physics_process(delta):
	if not lock_animation:
		if _animated_sprite.animation != 'idle':
			_animated_sprite.play("idle")

#activated by signal damage
func on_hit():
	_animated_sprite.play("damage")
	lock_animation = true
	EntitiesControler._on_damage(get_instance_id())

#Activated by control on death
func on_death():
	_animated_sprite.play("death")
	lock_animation = true
	

func _on_AnimatedSprite_animation_finished():
	if lock_animation:
	
		if _animated_sprite.animation != "death":
			lock_animation = false
		else:
			queue_free()
