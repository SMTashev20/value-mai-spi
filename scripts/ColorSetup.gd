extends Control

class_name ColorSetup

@onready var _actual_element_data: PTableManager.ElementData = PTableManager.ElementData.new()

var element_data: PTableManager.ElementData = PTableManager.ElementData.new():
	set(data):
		_set_element_data(data)
	get:
		return _actual_element_data

@export var element_model_view: Node3D
@export var periodic_table_root: Control

@onready var base_mesh: MeshInstance3D = element_model_view.get_node(^"Base")
@onready var symbol_text: MeshInstance3D = element_model_view.get_node(^"Base/SymbolText")
@onready var name_text: MeshInstance3D = element_model_view.get_node(^"Base/NameText")
@onready var atomic_mass_text: MeshInstance3D = element_model_view.get_node(^"Base/AtomicMassText")
@onready var atomic_number_text: MeshInstance3D = element_model_view.get_node(^"Base/AtomicNumberText")

@export var background_texture: TextureRect
@export var name_label: Label
@export var description_label: RichTextLabel
@export var vertical_carousel: VerticalCarousel

var elements: Array;

func _set_element_data(data: PTableManager.ElementData):
	(symbol_text.material_override as StandardMaterial3D).albedo_color.r = data.color.r
	(symbol_text.material_override as StandardMaterial3D).albedo_color.g = data.color.g
	(symbol_text.material_override as StandardMaterial3D).albedo_color.b = data.color.b
	
	(base_mesh.material_override as StandardMaterial3D).albedo_color.r = data.color.lightened(.75).r
	(base_mesh.material_override as StandardMaterial3D).albedo_color.g = data.color.lightened(.75).g
	(base_mesh.material_override as StandardMaterial3D).albedo_color.b = data.color.lightened(.75).b
	
	(symbol_text.mesh as TextMesh).text = data.symbol
	(name_text.mesh as TextMesh).text = data.name
	(atomic_mass_text.mesh as TextMesh).text = str(data.atomic_mass)
	(atomic_number_text.mesh as TextMesh).text = str(data.atomic_number)
	
	(background_texture.texture as GradientTexture2D).gradient.set_color(0, data.color.lightened(.5))
	(background_texture.texture as GradientTexture2D).gradient.set_color(1, data.color.lightened(.99))
	
	name_label.text = data.name.to_lower()
	name_label.theme.set_color("font_color", "Label", data.color)
	
	description_label.text = data.description
	
	vertical_carousel.current_number = data.atomic_number
	var carousel_children = vertical_carousel.get_children()
	(carousel_children[carousel_children.size() / 2] as Label).label_settings.font_color = data.color

func _ready():
	pass

func _on_vertical_carousel_number_changed(new_val, old_val):
	var periodic_table = periodic_table_root.get_node("CenterContainer/GridContainer") as PTableManager
	
	# shhhhhh
	if new_val == 0:
		var data = PTableManager.ElementData.new()
		
		data.atomic_number = 0
		data.name = "Defaultium"
		data.symbol = "De"
		data.shells = []
		data.atomic_mass = 0.0
		data.color = Color(.5, .5, .5, 1.)
		data.button_theme = null
		data.relative_pos = Vector2i(0, 0)
		data.description = "The default element is quite strange. It always leads to undefined behaviour when reacting with other elements, but people can't live without it because it serves an important purpose. It represents nothing."
		
		element_data = data
		
		return
	
	if new_val > periodic_table.element_array.size(): return
	element_data = periodic_table.gen_element_data(periodic_table.element_array[new_val - 1])
