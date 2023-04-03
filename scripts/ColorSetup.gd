extends Control

class_name ColorSetup

@onready var _actual_element_data: PTableManager.ElementData = PTableManager.ElementData.new()

var element_data: PTableManager.ElementData = PTableManager.ElementData.new():
	set(data):
		_set_element_data(data)
	get:
		return _actual_element_data

@export var element_model_view: ElementModelViewManager
@export var periodic_table_root: Control

@export var background_texture: TextureRect
@export var name_label: Label
@export var description_label: RichTextLabel
@export var vertical_carousel: VerticalCarousel
@export var electron_shell_visualiser: ElectronShellVis
@export var additional_properties_bar: VBoxContainer

@export var additional_property_even_theme: Theme
@export var additional_property_odd_theme: Theme

var elements: Array;

func _set_element_data(data: PTableManager.ElementData):
	element_model_view.update_colors(data.color)
	element_model_view.update_texts(data)
	
	(background_texture.texture as GradientTexture2D).gradient.set_color(0, data.color.lightened(.5))
	(background_texture.texture as GradientTexture2D).gradient.set_color(1, data.color.lightened(.99))
	
	name_label.text = data.name.to_lower()
	name_label.theme.set_color("font_color", "Label", data.color)
	
	description_label.text = data.description
	
	vertical_carousel.current_number = data.atomic_number
	var carousel_children = vertical_carousel.get_children()
	(carousel_children[carousel_children.size() / 2] as Label).label_settings.font_color = data.color
	
	electron_shell_visualiser.shells = data.shells
	for i in range(10): electron_shell_visualiser.shells.append(0)
	electron_shell_visualiser.queue_redraw()
	
	electron_shell_visualiser.core_color = data.color.darkened(.25)
	electron_shell_visualiser.electron_color = data.color.lightened(.25)
	
	(additional_property_even_theme.get_stylebox("normal", "Label") as StyleBoxFlat).bg_color = data.color.lightened(.75)
	(additional_property_odd_theme.get_stylebox("normal", "Label") as StyleBoxFlat).bg_color = data.color.lightened(.65)
	
	(additional_property_even_theme.get_stylebox("normal", "Label") as StyleBoxFlat).bg_color.a = .5
	(additional_property_odd_theme.get_stylebox("normal", "Label") as StyleBoxFlat).bg_color.a = .5
	
	for child in additional_properties_bar.get_children():
		additional_properties_bar.remove_child(child)
	
	var counter = 0
	for additional_property in data.additional_properties:
		if data.additional_properties[additional_property] == null: continue
		
		var label = Label.new()
		
		if counter % 2 == 0: label.theme = additional_property_even_theme
		else: label.theme = additional_property_odd_theme
		
		label.text = additional_property
		label.text += ": "
		label.text += data.additional_properties[additional_property]
		additional_properties_bar.add_child(label)
		
		counter += 1

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
		data.shells = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
		data.atomic_mass = 0.0
		data.color = Color(.5, .5, .5, 1.)
		data.button_theme = null
		data.relative_pos = Vector2i(0, 0)
		data.description = "The default element is quite strange. It always leads to undefined behaviour when reacting with other elements, but people can't live without it because it serves an important purpose. It represents nothing."
		
		element_data = data
		
		return
		
	if new_val == 420:
		var data = PTableManager.ElementData.new()
		
		data.atomic_number = 420
		data.name = "MLGium"
		data.symbol = "Mlg"
		data.shells = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
		data.atomic_mass = 420.69
		data.color = Color(.1, .1, .1, 1.)
		data.button_theme = null
		data.relative_pos = Vector2i(0, 0)
		data.description = "Uhhh what did you stumble upon? Did the developer do this?..."
		
		element_data = data
		
		return
		
	if new_val == 1000:
		var data = PTableManager.ElementData.new()
		
		data.atomic_number = 1000
		data.name = "Obamium"
		data.symbol = "Oba"
		data.shells = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
		data.atomic_mass = 10.0
		data.color = Color.from_string("8a5c3eff", Color(0., 0., 0., 1.))
		data.button_theme = null
		data.relative_pos = Vector2i(0, 0)
		data.description = "Obamium."
		
		element_data = data
		
		return
	
	if new_val > periodic_table.element_array.size() || new_val < 0: return
	element_data = periodic_table.gen_element_data(periodic_table.element_array[new_val - 1])


func _on_line_edit_text_submitted(new_text: String):
	if int(new_text) == 0: _on_vertical_carousel_number_changed(1, -1)
	else: _on_vertical_carousel_number_changed(int(new_text), -1)
