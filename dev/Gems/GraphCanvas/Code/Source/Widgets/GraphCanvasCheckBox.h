/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#pragma once

#include <QGraphicsWidget>

#include <AzCore/Component/EntityId.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/std/string/string.h>

#include <Styling/definitions.h>
#include <Styling/StyleHelper.h>

namespace GraphCanvas
{
    //! The GraphCanvasCheck gives a QGraphicsWidget that is a check box.
    class GraphCanvasCheckBox
        : public QGraphicsWidget
    {
    public:
        AZ_CLASS_ALLOCATOR(GraphCanvasCheckBox, AZ::SystemAllocator, 0);

        GraphCanvasCheckBox(QGraphicsItem* parent = nullptr);
        ~GraphCanvasCheckBox() = default;        
        
        void SetStyle(const AZ::EntityId& entityId, const char* style = Styling::Elements::CheckBox);
        void SetSceneStyle(const AZ::EntityId& sceneId, const char* style = Styling::Elements::CheckBox);

        void SetChecked(bool checked);
        bool IsChecked() const;

    protected:
        // QGraphicsItem
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;             
        
        void mousePressEvent(QGraphicsSceneMouseEvent* mmouseEvent) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

        void hoverEnterEvent(QGraphicsSceneHoverEvent* hoverEvent) override;
        void hoverLeaveEvent(QGraphicsSceneHoverEvent* hoverEvent) override;
        ////

        // QGraphicsLayoutItem
        QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint = QSizeF()) const override;
        ////

    private:
    
        Styling::StyleHelper m_styleHelper;
    
        bool m_checked;
        bool m_pressed;
    };

    class GraphCanvasCheckBoxNotifications 
        : public AZ::EBusTraits
    {
    public:
        static const AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = GraphCanvasCheckBox*;

        virtual void OnValueChanged(bool checked) = 0;
        virtual void OnClicked() = 0;
    };

    using GraphCanvasCheckBoxNotificationBus = AZ::EBus<GraphCanvasCheckBoxNotifications>;
}