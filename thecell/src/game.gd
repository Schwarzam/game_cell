extends Spatial

var player = preload("res://src/player/Player.tscn")

onready var Entities = $Entities

func _ready():
	if global.LOBBY_ID != 0:
		global._send_P2P_Packet(0, {"test": "teste"})
		for PLAYER in global.LOBBY_MEMBERS:
			print("Player: ", PLAYER)
			var MASTER = (global.STEAM_ID == PLAYER['steam_id'])

			instance_player_at_location(PLAYER['steam_id'], MASTER)
			
	else:
		instance_player_at_location(0, false)

func _process(delta):
	if global.LOBBY_ID > 0:
		_read_P2P_Packet()

func instance_player_at_location(id, MASTER: bool = false, location: Vector3 = Vector3(4, 2, 4)):
	var player_instance = player.instance()
	player_instance.MASTER = MASTER
	
	Entities.add_child(player_instance)
	
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
		# Append logic here to deal with packet data
		if READABLE['message'] == "start":
			print("[STEAM] Starting P2P game...\n")



