/**
 * @file LayerStack.h
 * @brief Defines the LayerStack class for managing application layers.
 * 
 * @details The LayerStack manages the order of layers and overlays in the application. Layers
 * are rendered and updated in the order they are pushed, while overlays are always
 * rendered on top.
 */
#pragma once

namespace Ares {

	class Layer;

	/**
	 * @class LayerStack
	 * @brief Manages the stack of [Layers](#Ares::Layer) in the Application.
	 * 
	 * @details Layers are stored in a [Vector](#Ares::Vector), with overlays added to the end of the stack.
	 * Layers and overlays can be dynamically added or removed during runtime.
	 */
	class LayerStack
	{
	private:
		/**
		 * @brief Constructs a LayerStack.
		 */
		LayerStack();

	public:

		/**
		 * @brief Destroys the LayerStack and cleans up all layers.
		 */
		~LayerStack();

		/**
		 * @brief Adds a layer to the stack.
		 * 
		 * @param layer The layer to add.
		 */
		void PushLayer(Ref<Layer> layer);

		/**
		 * @brief Adds an overlay to the stack.
		 * 
		 * @param overlay The overlay to add.
		 */
		void PushOverlay(Ref<Layer> overlay);

		/**
		 * @brief Removes a layer from the stack.
		 * 
		 * @param layer The layer to remove.
		 */
		void PopLayer(Ref<Layer> layer);

		/**
		 * @brief Removes an overlay from the stack.
		 * 
		 * @param overlay The overlay to be removed.
		 */
		void PopOverlay(Ref<Layer> overlay);

		/**
		 * @brief Retrieves an iterator to the beginning of the layer stack.
		 * 
		 * @return An iterator pointing to the first layer.
		 */
		Vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }

		/**
		 * @brief Retrieves an iterator to the end of the layer stack.
		 * 
		 * @return An iterator pointing to the last layer.
		 */
		Vector<Ref<Layer>>::iterator end() { return m_Layers.end(); }

		/**
		 * @brief Retrieves a reverse iterator to the beginning of the stack.
		 * 
		 * @return A reverse iterator pointing to the last layer.
		 */
		Vector<Ref<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }

		/**
		 * @brief Retrieves a reverse iterator to the end of the stack.
		 * 
		 * @return A reverse iterator pointing before the first layer.
		 */
		Vector<Ref<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }

		/**
		 * @brief Retrieves a constant iterator to the beginning of the stack.
		 * 
		 * @return A constant iterator pointing to the first layer.
		 */
		Vector<Ref<Layer>>::const_iterator begin() const { return m_Layers.begin(); }

		/**
		 * @brief Retrieves a constant iterator to the end of the stack.
		 * 
		 * @return A constant iterator pointing pas the last layer.
		 */
		Vector<Ref<Layer>>::const_iterator end() const { return m_Layers.end(); }

		/**
		 * @brief Retrieves a constant reverse iterator to the beginning of the stack.
		 * 
		 * @return A constant reverse iterator pointing to the last layer.
		 */
		Vector<Ref<Layer>>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		
		/**
		 * @brief Retrieves a constant reverse iterator to the end of the stack.
		 * 
		 * @return A constant reverse iterator pointing before the first layer.
		 */
		Vector<Ref<Layer>>::const_reverse_iterator rend() const { return m_Layers.rend(); }

		/**
		 * @brief Creates an instance of the LayerStack class. Called during Application construction.
		 * 
		 * @details This static method creates an instance of the LayerStack class.
		 * 
		 * @return A Scope to the created LayerStack object.
		 */
		static Scope<LayerStack> Create();

	private:
		template <typename ObjectType, typename... Args>
		friend Scope<ObjectType> Ares::CreateScope(Args&&... args);

	private:
		Vector<Ref<Layer>> m_Layers;		///< Vector containing all layers and overlays.
		uint32_t m_LayerInsertIndex = 0;	///< Index indicating where layers end and overlays begin.
	};

}