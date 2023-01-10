extends Spatial


func _ready():
	$Particles.visible = false
	$Particles2.visible = false
	if not get_parent().get_parent().MASTER:
		$GunSkin.set_layer_mask_bit(0, true)
		$GunSkin.set_layer_mask_bit(1, false)


func shoot():
	$AnimationPlayer.play("Kick")
	

func reload():
	pass
