# Egaku source: `Editor/RenderGraph/NodeEdgeConnector.cs`

> 自动从 `EgakuRenderPipeline.zip` 的文本源码转译。该实现是 Unity 参考，不决定 UE5 架构。

```csharp
﻿using UnityEditor.Experimental.GraphView;
using UnityEngine;
using UnityEngine.UIElements;

namespace Egaku.Rendering.Pipeline.Editor {


  public class NodeEdgeConnector<TEdge> : EdgeConnector where TEdge : Edge, new() {
    private readonly EdgeDragHelper m_EdgeDragHelper;
    private Edge m_EdgeCandidate;
    private bool m_Active;
    private Vector2 m_MouseDownPosition;
    internal const float k_ConnectionDistanceTreshold = 10f;

    public NodeEdgeConnector(IEdgeConnectorListener listener) {
      m_EdgeDragHelper = new EdgeDragHelper<TEdge>(listener);
      m_Active = false;
      activators.Add(new ManipulatorActivationFilter() {
        button = MouseButton.LeftMouse
      });
    }

    public override EdgeDragHelper edgeDragHelper => this.m_EdgeDragHelper;

    protected override void RegisterCallbacksOnTarget() {
      target.RegisterCallback<MouseDownEvent>(OnMouseDown);
      target.RegisterCallback<MouseMoveEvent>(OnMouseMove);
      target.RegisterCallback<MouseUpEvent>(OnMouseUp);
      target.RegisterCallback<KeyDownEvent>(OnKeyDown);
      target.RegisterCallback<MouseCaptureOutEvent>(OnCaptureOut);
    }

    protected override void UnregisterCallbacksFromTarget() {
      target.UnregisterCallback<MouseDownEvent>(OnMouseDown);
      target.UnregisterCallback<MouseMoveEvent>(this.OnMouseMove);
      target.UnregisterCallback<MouseUpEvent>(OnMouseUp);
      target.UnregisterCallback<KeyDownEvent>(OnKeyDown);
    }

    protected virtual void OnMouseDown(MouseDownEvent e) {
      if (this.m_Active) {
        Debug.Log("OnMouseDown Active");
        e.StopImmediatePropagation();
      } else {
        Debug.Log("OnMouseDown not Active");
        if (!CanStartManipulation(e) || !(this.target is Port target)) {
            Debug.Log("OnMouseDown target not manipulatable");
            return;
        }
      
        this.m_MouseDownPosition = e.localMousePosition;
        this.m_EdgeCandidate = (Edge)new TEdge();
        this.m_EdgeDragHelper.draggedPort = target;
        this.m_EdgeDragHelper.edgeCandidate = this.m_EdgeCandidate;
        if (this.m_EdgeDragHelper.HandleMouseDown(e)) {
          this.m_Active = true;
          this.target.CaptureMouse();
          e.StopPropagation();
        } else {
          this.m_EdgeDragHelper.Reset();
          this.m_EdgeCandidate = (Edge)null;
        }
      }
    }

    private void OnCaptureOut(MouseCaptureOutEvent e) {
      this.m_Active = false;
      if (this.m_EdgeCandidate == null)
        return;
      this.Abort();
    }

    protected virtual void OnMouseMove(MouseMoveEvent e) {
      if (!this.m_Active)
        return;
      this.m_EdgeDragHelper.HandleMouseMove(e);
      this.m_EdgeCandidate.candidatePosition = e.mousePosition;
      this.m_EdgeCandidate.UpdateEdgeControl();
      e.StopPropagation();
    }

    protected virtual void OnMouseUp(MouseUpEvent e) {
      Debug.Log("OnMouseUp.");
      if (!this.m_Active || !this.CanStopManipulation((IMouseEvent)e)) {
        Debug.Log("OnMouseUp not active.");
        return;
      }

      if (this.CanPerformConnection(e.localMousePosition))
        this.m_EdgeDragHelper.HandleMouseUp(e);
      else
        this.Abort();
      this.m_Active = false;
      this.m_EdgeCandidate = (Edge)null;
      this.target.ReleaseMouse();
      e.StopPropagation();
    }

    private void OnKeyDown(KeyDownEvent e) {
      if (e.keyCode != KeyCode.Escape || !this.m_Active)
        return;
      this.Abort();
      this.m_Active = false;
      this.target.ReleaseMouse();
      e.StopPropagation();
    }

    protected virtual void Abort() {
      this.target?.GetFirstAncestorOfType<UnityEditor.Experimental.GraphView.GraphView>()
        ?.RemoveElement((GraphElement)this.m_EdgeCandidate);
      this.m_EdgeCandidate.input = (Port)null;
      this.m_EdgeCandidate.output = (Port)null;
      this.m_EdgeCandidate = (Edge)null;
      this.m_EdgeDragHelper.Reset();
    }

    private bool CanPerformConnection(Vector2 mousePosition) {
      return (double)Vector2.Distance(this.m_MouseDownPosition, mousePosition) > 10.0;
    }
  }
  
}
```
