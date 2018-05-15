from sklearn import datasets

#iris = datasets.load_iris()
#data = iris.data[:, [0, 2]]
#label = iris.target
import pickle
PDR = []
RSSI = []
label = []
with open("data.bin","rb") as f:
    PDR = pickle.load(f)
    RSSI = pickle.load(f)
    label = pickle.load(f)

print label

from sklearn.model_selection import train_test_split

data0 = [[PDR[i],RSSI[i]] for i in range(len(PDR)) if label[i]==0]
data1 = [[PDR[i],RSSI[i]] for i in range(len(PDR)) if label[i]==1]
data2 = [[PDR[i],RSSI[i]] for i in range(len(PDR)) if label[i]==2]
data3 = [[PDR[i],RSSI[i]] for i in range(len(PDR)) if label[i]==3]
data0_train,  data0_test = train_test_split(data0, test_size=0.3)
data1_train,  data1_test = train_test_split(data1, test_size=0.3)
data2_train,  data2_test = train_test_split(data2, test_size=0.3)
data3_train,  data3_test = train_test_split(data3, test_size=0.3)
data_train = data0_train+data1_train+data2_train+data3_train
data_test = data0_test+data1_test+data2_test+data3_test
label_train = [0 for i in range(len(data0_train))]+[1 for i in range(len(data1_train))]+[2 for i in range(len(data2_train))]+[3 for i in range(len(data3_train))]
label_test = [0 for i in range(len(data0_test))]+[1 for i in range(len(data1_test))]+[2 for i in range(len(data2_test))]+[3 for i in range(len(data3_test))]
#label = []


#data_train,  data_test, label_train, label_test = train_test_split(data, label, test_size=0.3)
from sklearn.metrics import accuracy_score
from sklearn.externals import joblib

# svm
from sklearn import svm
#clf = svm.SVC(gamma=0.001, C=100.)
clf = svm.SVC(kernel="rbf", C=1,gamma=0.001)
clf.fit(data_train, label_train)
print(accuracy_score(clf.predict(data_test), label_test))
joblib.dump(clf, 'svm.pkl')

"""
import numpy as np
import matplotlib.pyplot as plt
x_min, x_max = min(PDR) - 1, max(PDR) + 1
y_min, y_max = min(RSSI) - 1, max(RSSI) + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.1),
                     np.arange(y_min, y_max, 0.1))

Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
Z = Z.reshape(xx.shape)

plt.contourf(xx, yy, Z, alpha=0.4)
plt.scatter(PDR, RSSI, c=label, alpha=0.8)
plt.show()
"""

# decision tree
from sklearn.tree import DecisionTreeClassifier
clf = DecisionTreeClassifier(max_depth=4)
clf.fit(data_train, label_train)
print(accuracy_score(clf.predict(data_test), label_test))
joblib.dump(clf, 'DT.pkl')

"""
import numpy as np
import matplotlib.pyplot as plt
x_min, x_max = min(PDR) - 1, max(PDR) + 1
y_min, y_max = min(RSSI) - 1, max(RSSI) + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.1),
                     np.arange(y_min, y_max, 0.1))

Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
Z = Z.reshape(xx.shape)

plt.contourf(xx, yy, Z, alpha=0.4)
plt.scatter(PDR, RSSI, c=label, alpha=0.8)
plt.show()
"""

#Random forests
from sklearn.ensemble import RandomForestClassifier
clf = RandomForestClassifier(n_estimators=10)
clf = clf.fit(data_train, label_train)
print(accuracy_score(clf.predict(data_test), label_test))
joblib.dump(clf, 'RF.pkl')

import numpy as np
import matplotlib.pyplot as plt
x_min, x_max = min(PDR) - 1, max(PDR) + 1
y_min, y_max = min(RSSI) - 1, max(RSSI) + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, 0.1),
                     np.arange(y_min, y_max, 0.1))

Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
Z = Z.reshape(xx.shape)

plt.contourf(xx, yy, Z, alpha=0.4)
plt.scatter(PDR, RSSI, c=label, alpha=0.8)
plt.show()
