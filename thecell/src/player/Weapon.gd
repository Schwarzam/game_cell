extends Node

class_name Weapon

var weapon_name = "sniper"
export var fire_rate = 0.1
export var clip_size = 12
export var reload_rate = 1

var current_ammo = clip_size

var can_fire = true
var reloading = false

onready var ammo_label : Label = $"../UI/Label"
onready var raycast : RayCast = $"../Head/Camera/WeaponRC"

var MASTER = false

func _ready():
	MASTER = get_parent().MASTER

func _process(delta):
	if MASTER:
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
	if MASTER:
		if raycast.is_colliding():
			var collider = raycast.get_collider()
			print("Shot at: ", collider)
			
			if collider.is_in_group("Enemies"):
				global._send_P2P_Packet(0, {"damage": 30, "tg": collider.name})
				collider.take_damage(30)

func fire():
	global._send_P2P_Packet(0, {"shot": weapon_name}, Steam.P2P_SEND_UNRELIABLE_NO_DELAY)
	can_fire = false
	get_parent().get_node("Head/Gun").shoot()
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
	
	
