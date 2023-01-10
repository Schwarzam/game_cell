extends KinematicBody


export var speed = 1

var path = []
var cur_path_idx = 0
var velocity = Vector3.DOWN
var threshold = .1

var damage_timer = Timer.new() 
# Called when the node enters the scene tree for the first time.

var attacking = false
var life = 100

var last_transform_origin = null

onready var target = null
onready var agent : NavigationAgent = $NavAgent

onready var tween : Tween = $Tween

var dead = false

func _ready():
	damage_timer.connect("timeout", self, "damage_agent") 
	add_child(damage_timer)
	
	$Persona/AnimationPlayer.play("idle")
	$Persona/AnimationPlayer.playback_speed = 1.7
	#agent.set_navigation(nav_map)

func _physics_process(delta):
	if dead:
		return
	if target:
		if global.host or not tween.is_active():
			if not attacking:
				move_and_slide(velocity, Vector3.UP)
				
				if $Persona/AnimationPlayer.assigned_animation != "walking":
					$Persona/AnimationPlayer.play("walking")
	
		var look_direction = Vector2(velocity.z, velocity.x)
		$Persona.rotation.y =  lerp($Persona.rotation.y, look_direction.angle(), 0.5)
		
		if agent.distance_to_target() < 1 and not attacking:
			attack()	


func _set_target(targ):
	target = targ
	agent.set_target_location(target.transform.origin)


func _on_Timer_timeout():
	if dead:
		return
	if global.host and target:
		agent.set_target_location(target.transform.origin)
		velocity = (agent.get_next_location() - transform.origin).normalized() * speed
	elif target:
		agent.set_target_location(target.transform.origin)
	else:
		$Persona/AnimationPlayer.play("idle")


func take_damage(damage):
	life -= damage
	if life < 0:
		die()

func die():
	dead = true
	$Persona/AnimationPlayer.play("die")
	$CollisionShape.disabled = true

func attack_damage_timer(time):
	damage_timer.wait_time = time
	damage_timer.one_shot = true
	damage_timer.start()
	
func damage_agent():
	if target.transform.origin.distance_to(transform.origin) < 1.3:
		if target.MASTER:
			global._send_P2P_Packet(0, {"damage": 20, "tg": target.name})
			target.take_damage(20)
			print("TOMOU")

func _on_attack_animation_finished():
	print("FINISHED")
	attacking = false


func attack():
	attacking = true
	$Persona/AnimationPlayer.play("attack")
	attack_damage_timer(0.7)

func set_position(new_value):
	tween.interpolate_property(self, "global_transform:origin", global_transform.origin, new_value, 0.1)
	tween.start()

func _on_Network_tick_rate_timeout():
	if $Persona/AnimationPlayer.assigned_animation == "idle":
		return
		
	if global.host and target:
		global._send_P2P_Packet(0, {"z_n": name, "z_ps": global_transform.origin, "z_vl": velocity, "z_tg": target.name})
	elif global.host and not target:
		global._send_P2P_Packet(0, {"z_n": name, "z_ps": global_transform.origin, "z_vl": velocity, "z_tg": null})
