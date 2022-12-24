extends Control


func _enter_tree():
	if OS.get_name() != 'OSX':
		var Steam = preload("res://addons/godotsteam/godotsteam.gdns").new()
	
var SteamName = ""
# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	Steam.connect("avatar_loaded", self, "_loaded_avatar")
	Steam.getPlayerAvatar(2) ## 2 - medium
	
	SteamName = Steam.getPersonaName()
	$CanvasLayer/Label.text = SteamName
	
	

func _loaded_avatar(id: int, size: int, buffer: PoolByteArray) -> void:
	print("Avatar for user: "+str(id))
	print("Size: "+str(size))
	# Create the image and texture for loading
	var AVATAR = Image.new()
	var AVATAR_TEXTURE: ImageTexture = ImageTexture.new()
	AVATAR.create_from_data(size, size, false, Image.FORMAT_RGBA8, buffer)
	# Apply it to the texture
	AVATAR_TEXTURE.create_from_image(AVATAR)
	# Set it
	
	$CanvasLayer/Sprite.set_texture(AVATAR_TEXTURE)
	
	
	
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass



func _on_Quit_pressed():
	get_tree().quit()


func _on_Play_pressed():
	EntitiesControler.GAME_RUNNING = true
	get_tree().change_scene("res://Game/Game.tscn")
