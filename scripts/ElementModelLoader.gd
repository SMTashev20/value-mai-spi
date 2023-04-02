extends MeshInstance3D

class_name ElementModelLoader

@onready var _actual_element_data: PTableManager.ElementData = PTableManager.ElementData.new()

var element_data: PTableManager.ElementData = PTableManager.ElementData.new():
	set(data):
		_set_element_data(data)
	get:
		return _actual_element_data

@export var symbol_text: MeshInstance3D
@export var name_text: MeshInstance3D
@export var atomic_mass_text: MeshInstance3D
@export var atomic_number_text: MeshInstance3D

@export_range(0.0, 180.0, 1.0) var rotation_range = 75.0;

@onready var window_res = Vector2(get_window().size)

func _set_element_data(data: PTableManager.ElementData):
	(symbol_text.material_override as StandardMaterial3D).albedo_color.r = data.color.r
	(symbol_text.material_override as StandardMaterial3D).albedo_color.g = data.color.g
	(symbol_text.material_override as StandardMaterial3D).albedo_color.b = data.color.b
	
	(material_override as StandardMaterial3D).albedo_color.r = data.color.lightened(.25).r
	(material_override as StandardMaterial3D).albedo_color.g = data.color.lightened(.25).g
	(material_override as StandardMaterial3D).albedo_color.b = data.color.lightened(.25).b
	
	(symbol_text.mesh as TextMesh).text = data.symbol
	(name_text.mesh as TextMesh).text = data.name
	(atomic_mass_text.mesh as TextMesh).text = str(data.atomic_mass)
	(atomic_number_text.mesh as TextMesh).text = str(data.atomic_number)

func _ready():
	pass


func _process(delta):
	var mouse_pos = get_viewport().get_mouse_position()
	var uv = (mouse_pos / window_res - Vector2(.5, .5))
	
	uv.x = clampf(uv.x, -.5, .5)
	uv.y = clampf(uv.y, -.5, .5)
	
	rotation_degrees.x = rotation_range * uv.y + 90
	rotation_degrees.y = rotation_range * uv.x
