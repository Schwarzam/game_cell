extends Spatial

var player = preload("res://src/player/Player.tscn")


func _ready():
	if global.LOBBY_ID != 0:
		global._send_P2P_Packet(0, {"test": "teste"})
		for PLAYER in global.LOBBY_MEMBERS:
			print("Player: ", PLAYER)
			var MASTER = (global.STEAM_ID == PLAYER['steam_id'])

			instance_player_at_location(PLAYER['steam_id'], MASTER)
			
	else:
		#Debug case
		global.host = true
		instance_player_at_location(0, true)

func _process(delta):
	if global.LOBBY_ID > 0:
		_read_P2P_Packet()

func instance_player_at_location(id, MASTER: bool = false, location: Vector3 = Vector3(-4, 2, -4)):
	var player_instance = player.instance()
	player_instance.MASTER = MASTER
	
	$Map.spawn(player_instance)
	
	player_instance.global_transform.origin = location
	player_instance.name = str(id)


# Read a Steam P2P packet
func _read_P2P_Packet() -> void:
	var PACKET_SIZE: int = Steam.getAvailableP2PPacketSize(0)
	# There is a packet
	if PACKET_SIZE > 0:
		print("[STEAM] There is a packet available.")
		# Get the packet
		var PACKET: Dictionary = Steam.readP2PPacket(PACKET_SIZE, 0)
		# If it is empty, set a warning
		if PACKET.empty():
			print("[WARNING] Read an empty packet with non-zero size!")
		# Get the remote user's ID
		var PACKET_SENDER: String = str(PACKET['steam_id_remote'])
		var PACKET_CODE: PoolByteArray = PACKET['data']
		# Make the packet data readable
		var READABLE: Dictionary = bytes2var(PACKET_CODE)
		# Print the packet to output
		print("[STEAM] Packet from "+str(PACKET_SENDER)+": "+str(READABLE)+"\n")
		update_player(PACKET_SENDER, READABLE)

func update_player(id, data : Dictionary):
	if data.has("z_n"):
		var node = $Map.get_entity_node(data["z_n"])
		if data.has("z_ps"):
			node.set_position(data["z_ps"])
		if data.has("z_vl"):
			node.velocity = data["z_vl"]
		if data.has("z_tg"):
			node.target = $Map.get_entity_node(data["z_tg"])
	elif data.has("damage") and data.has("tg"):
		var node = $Map.get_entity_node(data["tg"])
		node.take_damage(int(data["damage"]))
	else:
		#update players
		var node = $Map.get_entity_node(id)
		if not node:
			print("No node found")
			return
		if data.has("ps"):
			node.set_position(data["ps"])
		if data.has("rt"):
			node.global_rotation.y = data["rt"].x
			node.head.rotation.x = data["rt"].y
		if data.has("vl"):
			node.velocity = data["vl"]
	
	
		



