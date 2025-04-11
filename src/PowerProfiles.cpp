/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
 *
 * Copyright: 2025~ LXQt team
 * Authors:
 *   Palo Kisa <palo.kisa@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include <QMenu>
#include <QActionGroup>
#include <QAction>
#include <QThread>
#include <QDBusMetaType>
#include "PowerProfiles.h"
#include "DBusPropAsyncGetter.h"

namespace {
    using Trans = struct { char const * const s1; char const * const s2; };
    [[maybe_unused]] void just_for_translations_dummy_function(const Trans = QT_TRANSLATE_NOOP3("LXQt::PowerProfiles", "power-saver", "power-profiles-daemon")
            , const Trans = QT_TRANSLATE_NOOP3("LXQt::PowerProfiles", "balanced", "power-profiles-daemon")
            , const Trans = QT_TRANSLATE_NOOP3("LXQt::PowerProfiles", "performance", "power-profiles-daemon")
            )
    {}
}

namespace LXQt
{
    Q_GLOBAL_STATIC(LXQt::PowerProfiles, g_power_profiles)

    const QString PowerProfiles::msDBusPPService = QStringLiteral("org.freedesktop.UPower.PowerProfiles");
    const QString PowerProfiles::msDBusPPPath = QStringLiteral("/org/freedesktop/UPower/PowerProfiles");
    const QString PowerProfiles::msDBusPPInterface = msDBusPPService;
    const QString PowerProfiles::msDBusPPProperties[] = { QStringLiteral("ActiveProfile"), QStringLiteral("Profiles") };

    PowerProfiles & PowerProfiles::instance()
    {
        return *g_power_profiles;
    }

    PowerProfiles::PowerProfiles(QObject * parent/* = nullptr*/)
        : QObject{parent}
        , mThread{new QThread}
        , mPropGetter{new DBusPropAsyncGetter{msDBusPPService, msDBusPPPath, msDBusPPInterface, QDBusConnection::systemBus()}}
        , mMenu{new QMenu}

    {
        mMenu->menuAction()->setIcon(QIcon::fromTheme(QStringLiteral("preferences-system-performance")));
        mMenu->setTitle(tr("Power profile"));

        // register the DBus types
        qRegisterMetaType<QListOfQVariantMap>("QListOfQVariantMap");
        qDBusRegisterMetaType<QListOfQVariantMap>();

        // we want to dispatch DBus calls on dedicated thread
        mPropGetter->moveToThread(mThread.get());

        // signals forwarding
        connect(mPropGetter.get(), &DBusPropAsyncGetter::fetched, this, &PowerProfiles::propertyFetched);
        connect(mPropGetter.get(), &DBusPropAsyncGetter::pushed, this, &PowerProfiles::propertyPushed);

        // interested signals for our processing
        connect(mPropGetter.get(), &DBusPropAsyncGetter::fetched, this, &PowerProfiles::onFetched);
        connect(mPropGetter.get(), &DBusPropAsyncGetter::serviceDisappeared, this, [this] { reassembleMenu({}); });

        // our generated signals for dispatching into dedicated thread
        connect(this, &PowerProfiles::needPropertyFetch, mPropGetter.get(), &DBusPropAsyncGetter::fetch);
        connect(this, &PowerProfiles::needPropertyPush, mPropGetter.get(), &DBusPropAsyncGetter::push);

        mThread->start();

        reassembleMenu({});

        Q_EMIT needPropertyFetch(msDBusPPProperties[PPP_ActiveProfile]);
        Q_EMIT needPropertyFetch(msDBusPPProperties[PPP_Profiles]);
    }

    PowerProfiles::~PowerProfiles()
    {
        mThread->quit();
        mThread->wait();
    }

    QMenu * PowerProfiles::menu()
    {
        return mMenu.get();
    }

    void PowerProfiles::onFetched(const QString name, const QVariant value)
    {
        if (name == msDBusPPProperties[PPP_ActiveProfile]) {
            const QString profile = qdbus_cast<QString>(value);
            if (mActiveProfile == profile)
                return;
            mActiveProfile = profile;
            if (mActions)
                for (auto const & action : mActions->actions())
                    action->setChecked(get<QString>(action->data()) == mActiveProfile);
        } else if (name == msDBusPPProperties[PPP_Profiles]) {
            reassembleMenu(qdbus_cast<QListOfQVariantMap>(value));
        }
    }

    void PowerProfiles::reassembleMenu(const QListOfQVariantMap & profiles)
    {
        mMenu->clear();
        if (profiles.empty())
        {
            mActions.reset(nullptr);
            mMenu->addAction(tr("power-profiles-daemon not available"))->setDisabled(true);
            return;
        }

        mActions.reset(new QActionGroup(nullptr));
        mActions->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);

        connect(mActions.get(), &QActionGroup::triggered, this, [this] (const QAction * a) {
            setActiveProfile(get<QString>(a->data()));
        });

        for (auto const & profile : profiles)
        {
            const QString p = qdbus_cast<QString>(profile[QStringLiteral("Profile")]);
            auto a = new QAction(tr(qUtf8Printable(p), "power-profiles-daemon"), mActions.get());
            a->setCheckable(true);
            a->setData(p);
            a->setChecked(mActiveProfile == p);
        }
        mMenu->addActions(mActions->actions());
    }

    const QString & PowerProfiles::activeProfile() const
    {
        return mActiveProfile;
    }

    void PowerProfiles::setActiveProfile(const QString &value)
    {
        // Note: Just emit the signal to make change. Don't modify any internal state.
        //  We will be notified by the fetched() signal upon sucessfull PropertyChanged.
        Q_EMIT needPropertyPush(msDBusPPProperties[PPP_ActiveProfile], QVariant::fromValue(QDBusVariant{value}));
    }
}
