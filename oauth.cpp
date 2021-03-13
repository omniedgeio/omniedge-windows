#include "oauth.h"
#include <QSettings>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDesktopServices>

OAuth::OAuth(QObject *parent) : QObject(parent)
{
    this->oAuthFlow = new QOAuth2AuthorizationCodeFlow(this);
    this->oAuthFlow->setScope("email");
    this->oAuthFlow->setScope("openid");

    // Get configurations
    QByteArray val;
    QFile file;
    file.setFileName(":/oauth.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        val = file.readAll();
        file.close();

        qDebug() << "OAuth: Opened configuration file.";

        QJsonDocument document = QJsonDocument::fromJson(val);
        QJsonObject object = document.object();
        QJsonObject settingsObject = object["dev"].toObject();

        // Client id and client secret
        QString clientId = settingsObject["client_id"].toString();
        QString clientSecret = settingsObject["client_secret"].toString();

        // Uris
        QUrl redirectUri(settingsObject["redirect_uri"].toString());
        QUrl authUri(settingsObject["cognito_uri"].toString() + "/login");
        QUrl tokenUri(settingsObject["cognito_uri"].toString() + "/oauth2/token");
        quint16 port = static_cast<quint16>(redirectUri.port());

        this->oAuthFlow->setAccessTokenUrl(tokenUri);
        this->oAuthFlow->setAuthorizationUrl(authUri);
        this->oAuthFlow->setClientIdentifier(clientId);
        this->oAuthFlow->setClientIdentifierSharedKey(clientSecret);

        qDebug() << "OAuth: Intialize settings successfully.";

        this->replyHandler = new OAuthReplyHandler(port, this);
        this->oAuthFlow->setReplyHandler(replyHandler);

        qDebug() << "OAuth: Intialize reply handler successfully.";

        connect(this->oAuthFlow, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, [=](QUrl url) {
            QDesktopServices::openUrl(url);
        });
        connect(this->oAuthFlow, &QOAuth2AuthorizationCodeFlow::granted, this, &OAuth::granted);
        connect(this->replyHandler, &OAuthReplyHandler::tokensReceived, this, &OAuth::tokensReceived);
    } else {
        qDebug() << "OAuth: Cannot open configuration file.";
    }
}

void OAuth::grant()
{
    qDebug() << "OAuth: grant";
    this->oAuthFlow->grant();
}

void OAuth::tokensReceived(const QVariantMap &data)
{
    QSettings settings;
    settings.setValue(SETTINGS_ID_TOKEN, data["id_token"]);
    settings.setValue(SETTINGS_ACCESS_TOKEN, data["access_token"]);
    settings.setValue(SETTINGS_REFRESH_TOKEN, data["refresh_token"]);
    settings.setValue(SETTINGS_EXPIRES_IN, QDateTime::currentSecsSinceEpoch() + data["expires_in"].toUInt());
    qDebug() << "OAuth: Token received.";
    qDebug() << "OAuth: ID TOKEN "      << (settings.contains(SETTINGS_ID_TOKEN) ? "Received" : "Not Received");
    qDebug() << "OAuth: ACCESS TOKEN "  << (settings.contains(SETTINGS_ACCESS_TOKEN) ? "Received" : "Not Received");
    qDebug() << "OAuth: REFRESH TOKEN " << (settings.contains(SETTINGS_REFRESH_TOKEN) ? "Received" : "Not Received");
    qDebug() << "OAuth: EXPIRES IN "    << QDateTime::fromSecsSinceEpoch(settings.value(SETTINGS_EXPIRES_IN).toUInt());
}

OAuth::~OAuth()
{
    delete this->oAuthFlow;
    delete this->replyHandler;
}
