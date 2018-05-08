from sklearn import datasets

iris = datasets.load_iris()
data = iris.data[:, [0, 2]]
label = iris.target
#data = []
#label = []

from sklearn.model_selection import train_test_split
data_train,  data_test, label_train, label_test = train_test_split(data, label, test_size=0.3, random_state=0)
from sklearn.metrics import accuracy_score
from sklearn.externals import joblib

# svm
from sklearn import svm
#clf = svm.SVC(gamma=0.001, C=100.)
clf = svm.SVC()
clf.fit(data_train, label_train)
print(accuracy_score(clf.predict(data_test), label_test))
joblib.dump(clf, 'svm.pkl')

# decision tree
from sklearn.tree import DecisionTreeClassifier
clf = DecisionTreeClassifier(max_depth=4)
clf.fit(data_train, label_train)
print(accuracy_score(clf.predict(data_test), label_test))
joblib.dump(clf, 'DT.pkl')

#Random forests
from sklearn.ensemble import RandomForestClassifier
clf = RandomForestClassifier(n_estimators=10)
clf = clf.fit(data_train, label_train)
print(accuracy_score(clf.predict(data_test), label_test))
joblib.dump(clf, 'RF.pkl')
