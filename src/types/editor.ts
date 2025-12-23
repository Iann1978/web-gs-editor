// Editor type definitions

export type Tool = 'select' | 'move' | 'rotate' | 'scale'

export type EntityType = 'mesh' | 'light' | 'camera' | 'gaussian' | 'group' | 'other'

export interface Transform {
  position: [number, number, number]
  rotation: [number, number, number, number] // quaternion
  scale: [number, number, number]
}

export interface Entity {
  id: string
  name: string
  type: EntityType
  transform: Transform
  visible: boolean
  locked: boolean
  children?: Entity[]
  parentId?: string
  // Additional properties for different entity types
  [key: string]: any
}

export interface HistoryEntry {
  id: string
  type: 'create' | 'delete' | 'modify' | 'transform'
  timestamp: number
  data: any
  undo: () => void
  redo: () => void
}

export interface PanelLayout {
  hierarchyWidth: number
  inspectorWidth: number
  hierarchyVisible: boolean
  inspectorVisible: boolean
  statusBarVisible: boolean
}

