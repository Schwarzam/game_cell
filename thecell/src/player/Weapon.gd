extends Node

class_name Weapon

export var fire_rate = 0.5
export var clip_size = 5
export var reload_rate = 1

var current_ammo = clip_size

var can_fire = true
var reloading = false

onready var ammo_label : Label = $"../UI/Label"
onready var raycast : RayCast = $"../Head/Camera/WeaponRC"

func _ready():
	pass # Replace with function body.

func _process(delta):
	if reloading:
		ammo_label.set_text("reloading")
	else:
		ammo_label.set_text("%d / %d" % [current_ammo, clip_size])
	
	if Input.is_action_just_pressed("primary_fire") and can_fire:
		if current_ammo > 0 and not reloading:
			fire()
		elif not reloading:
			reload()

func check_collision():
	if raycast.is_colliding():
		var collider = raycast.get_collider()
		print(collider)
		
		if collider.is_in_group("Enemies"):
			
			collider.queue_free()
			
			print("Kill")

func fire():
	can_fire = false
	check_collision()
	current_ammo -= 1
	yield(get_tree().create_timer(fire_rate), "timeout")
	
	can_fire = true

func reload():
	reloading = true
			
	yield(get_tree().create_timer(reload_rate), "timeout")
	current_ammo = clip_size
	
	reloading = false
	print("reload_complete")
	
	
