extends KinematicBody


export var speed = 5

var path = []
var cur_path_idx = 0
var velocity = Vector3.DOWN
var threshold = .1
# Called when the node enters the scene tree for the first time.

onready var target = get_tree().get_root().get_node("Game/Player")
onready var nav = get_tree().get_root().get_node("Game/Map/Navigation")

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
		move_and_slide(velocity, Vector3.UP)
		
func get_target_path(target_pos):
	path = nav.get_simple_path(global_transform.origin, target_pos)
	
	cur_path_idx = 0


func _on_Timer_timeout():
	print("tei")
	if target:
		get_target_path(target.global_transform.origin)
	
