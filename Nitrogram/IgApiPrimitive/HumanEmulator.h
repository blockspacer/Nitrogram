#ifndef HUMANEMULATOR_H
#define HUMANEMULATOR_H

#include "IgApi.h"
#include "IgObjects.h"

#include <QThread>

class HumanEmulator : public QThread {
    Q_OBJECT

public:
    HumanEmulator(const IgAccount&);

private:
    MediaItemList ExtractItems(const RequestReply &);

    void run();

    IgAccount igAccount;
    IgApi *igApi;
};

#endif // HUMANEMULATOR_H
