extends KinematicBody


export var speed = 5

var path = []
var cur_path_idx = 0
var velocity = Vector3.DOWN
var threshold = .1
# Called when the node enters the scene tree for the first time.

var attacking = false

onready var target = null
onready var nav = get_tree().get_root().get_node("Game/Map/Navigation")

func _ready():
	$Persona/AnimationPlayer.play("idle")
	$Persona/AnimationPlayer.playback_speed = 1.5

func _physics_process(delta):
	if path.size() > 0:
		move_to_target()
		
func move_to_target():
	if cur_path_idx >= path.size():
		return
	
	if global_transform.origin.distance_to(path[cur_path_idx]) < threshold:
		cur_path_idx += 1
	
	else:
		var direction = (path[cur_path_idx] - global_transform.origin)
		velocity = direction.normalized()
		
		print(velocity)
		
		if not attacking:
			move_and_slide(velocity, Vector3.UP)
			$Persona/AnimationPlayer.play("walking")
	
	var look_direction = Vector2(velocity.z, velocity.x)
	$Persona.rotation.y =  lerp($Persona.rotation.y, look_direction.angle(), 0.5)
	

func _set_target(targ):
	target = targ

		
func get_target_path(target_pos):
	path = nav.get_simple_path(global_transform.origin, target_pos)
	cur_path_idx = 0


func _on_Timer_timeout():
	if target:
		get_target_path(target.global_transform.origin)
	else:
		$Persona/AnimationPlayer.play("idle")


func _on_attack_animation_finished():
	attacking = false


func _on_Area_body_entered(body):
	if body.is_in_group("Players"):
		attacking = true
		$Persona/AnimationPlayer.play("punch")
