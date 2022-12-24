extends Node2D


onready var _animated_sprite = $AnimatedSprite

var attacking = false
var SPEED = 100
var motion = Vector2(0, 0)
var inputs = false

func _ready():
	_animated_sprite.connect("animation_finished", self, "_on_attack_finished")
	if inputs:
		$Camera2D.current = true
		
func run_action(action):
	if not self.attacking:
		_animated_sprite.play(action)

func get_animation():
	return _animated_sprite.animation

func _on_attack_finished():
	self.attacking = false

func _physics_process(delta):
	if self.attacking:
		if _animated_sprite.frame > 7 and _animated_sprite.frame < 11:
			get_node("ThunderAttack/direita").disabled = false
		else:
			get_node("ThunderAttack/direita").disabled = true
			
		if _animated_sprite.frame > 10 and _animated_sprite.frame < 14:
			get_node("ThunderAttack/esquerda").disabled = false
		else:
			get_node("ThunderAttack/esquerda").disabled = true

func attack():
	self.attacking = true
	_animated_sprite.play("attack")

func _on_ThunderAttack_body_entered(body):
	if body.has_method("on_hit"):
		body.on_hit()
