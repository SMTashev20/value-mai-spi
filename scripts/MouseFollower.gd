extends Node3D

@export_range(0.0, 180.0, 1.0, "deg") var rotation_range = 75.0

@onready var window_res = Vector2(get_window().size)
@onready var base_node: MeshInstance3D = $Base

func _ready():
	pass


func _process(delta):
	var mouse_pos = get_window().get_mouse_position()
	var uv = (mouse_pos / window_res - Vector2(.5, .5))
	
	uv.x = clampf(uv.x, -.5, .5)
	uv.y = clampf(uv.y, -.5, .5)
	
	base_node.rotation_degrees.x = rotation_range * uv.y + 90
	base_node.rotation_degrees.y = rotation_range * uv.x
